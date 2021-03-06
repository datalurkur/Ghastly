#include <Network/Socket.h>
#include <Base/Assertion.h>
#include <Base/Log.h>

bool Socket::InitializeSocketLayer() {
#if SYS_PLATFORM == PLATFORM_WIN32
    WSADATA wsaData;

    if(SocketLayerInitialized) { return true; }
    else if(WSAStartup(MAKEWORD(2,2), &wsaData) == NO_ERROR) {
        SocketLayerInitialized = true;
    }
#endif
    return IsSocketLayerReady();
}

void Socket::ShutdownSocketLayer() {
#if SYS_PLATFORM == PLATFORM_WIN32
    WSACleanup();
    SocketLayerInitialized = false;
#endif
}

bool Socket::IsSocketLayerReady() {
#if SYS_PLATFORM == PLATFORM_WIN32
    return SocketLayerInitialized;
#else
    return true;
#endif
}

int Socket::LastSocketError() {
#if SYS_PLATFORM == PLATFORM_WIN32
    return WSAGetLastError();
#else
    return errno;
#endif
}

bool Socket::SocketLayerInitialized = false;

Socket::Socket(bool blocking): _state(Uninitialized), _blocking(blocking), _socketHandle(0) {
    if(!IsSocketLayerReady()) {
        Warn("Socket layer not yet initialized! Please call InitializeSocketLayer if you expect your sockets to send data.");
    }
    _lock = SDL_CreateMutex();
}

Socket::~Socket() {
    closeSocket();

    if(_lock) {
        SDL_DestroyMutex(_lock);
        _lock = 0;
    }
}

bool Socket::createSocket(int type, int proto) {
    bool ret = true;

    if(_state != Uninitialized) {
        Error("Failed to create socket, socket has already been created.");
        return false;
    }

    // Create the socket
    SDL_LockMutex(_lock);
    _socketHandle = socket(AF_INET, type, proto);
    if(_socketHandle <= 0) {
        Error("Failed to create socket.");
        ret = false;
    } else {
        _state = Created;
    }
    SDL_UnlockMutex(_lock);

    setBlockingFlag(_blocking);

    return ret;
}

bool Socket::bindSocket(unsigned short localPort) {
    sockaddr_in addr;
    bool ret = true;

    if(_state != Created) {
        if(_state == Uninitialized) {
            Error("Failed to bind socket, socket not yet created");
        } else {
            Error("Failed to bind socket, socket has already been bound.");
        }
        return false;
    }

    // Bind the socket
    SDL_LockMutex(_lock);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(localPort);
    if(bind(_socketHandle, (const sockaddr*)&addr, sizeof(sockaddr_in)) < 0) {
        Error("Failed to bind socket to port " << localPort);
        ret = false;
    } else {
        _state = Bound;
    }
    SDL_UnlockMutex(_lock);

    return ret;
}

bool Socket::setBlockingFlag(bool value) {
#if SYS_PLATFORM == PLATFORM_WIN32
    DWORD nonBlock = (value ? 0 : 1);
    if(ioctlsocket(_socketHandle, FIONBIO, &nonBlock) != 0) {
#else
    int nonBlock = (value ? 0 : 1);
    if(fcntl(_socketHandle, F_SETFL, O_NONBLOCK, nonBlock) == -1) {
#endif
        Error("Failed to set socket non-blocking!");
        closeSocket();
        return false;
    } else {
        return true;
    }
}

void Socket::closeSocket() {
    SDL_LockMutex(_lock);

    if(_socketHandle) {
#if SYS_PLATFORM == PLATFORM_WIN32
        closesocket(_socketHandle);
#else
        close(_socketHandle);
#endif
        _socketHandle = 0;
        _state = Uninitialized;
    }

    SDL_UnlockMutex(_lock);
}

bool Socket::isOpen() {
    bool ret;

    SDL_LockMutex(_lock);
    ret = (_state == Bound || _state == Listening || _state == Connecting || _state == Connected);
    SDL_UnlockMutex(_lock);

    return ret;
}

unsigned short Socket::getLocalPort() {
    unsigned short ret;
    sockaddr_in addr;
    socklen_t addrSize = sizeof(addr);

    ASSERT(isOpen());

    SDL_LockMutex(_lock);
    if(getsockname(_socketHandle, (sockaddr*)&addr, &addrSize) == 0 && addr.sin_family == AF_INET && sizeof(addr) == addrSize) {
        ret = ntohs(addr.sin_port);
    } else {
        Error("Failed to get local port for Socket");
        ret = 0;
    }
    SDL_UnlockMutex(_lock);

    return ret;
}

bool Socket::send(const char *data, unsigned int size, const sockaddr *addr, int addrSize) {
    int bytesSent;

    ASSERT(isOpen());

    SDL_LockMutex(_lock);
    bytesSent = (int)sendto(_socketHandle, data, size, 0, addr, addrSize);
    SDL_UnlockMutex(_lock);

    if(bytesSent < 0) {
        Error("Failed to write to socket");
        return false;
    } else if(bytesSent != (int)size) {
        Error("Bytes sent does not match bytes given: " << bytesSent << "/" << size);
        return false;
    } else {
        return true;
    }
}

// WARNING: Any packets received that are larger than maxSize are SILENTLY discarded
// I know, right? How ridiculous is that? Thanks, OBAMA.
void Socket::recv(char *data, int &size, unsigned int maxSize, sockaddr *addr, int &addrSize) {
    ASSERT(isOpen());

    SDL_LockMutex(_lock);
    size = (int)recvfrom(_socketHandle, data, maxSize, 0, addr, (socklen_t*)&addrSize);
    SDL_UnlockMutex(_lock);
}
