#include <Network/Socket.h>
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
    bool ret;

    if(_state != Uninitialized) {
        Error("Failed to create socket, socket has already been created.");
        return false;
    }

    // Construct the socket type, including flags for nonblocking sockets
    if(!_blocking) { type |= SOCK_NONBLOCK; }

    // Create the socket
    SDL_mutexP(_lock);
    _socketHandle = socket(AF_INET, type, proto);
    if(_socketHandle <= 0) {
        Error("Failed to create socket.");
        ret = false;
    } else {
        _state = Created;
        ret = true;
    }
    SDL_mutexV(_lock);

    return ret;
}

bool Socket::bindSocket(unsigned short localPort) {
    sockaddr_in addr;
    bool ret;

    if(_state != Created) {
        if(_state == Uninitialized) {
            Error("Failed to bind socket, socket not yet created");
        } else {
            Error("Failed to bind socket, socket has already been bound.");
        }
        return false;
    }

    // Bind the socket
    SDL_mutexP(_lock);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(localPort);
    if(bind(_socketHandle, (const sockaddr*)&addr, sizeof(sockaddr_in)) < 0) {
        Error("Failed to bind socket to port " << localPort);
        ret = false;
    } else {
        _state = Bound;
        ret = true;
    }
    SDL_mutexV(_lock);

    return ret;
}

void Socket::closeSocket() {
    SDL_mutexP(_lock);

    if(_socketHandle) {
#if SYS_PLATFORM == PLATFORM_WIN32
        closesocket(_socketHandle);
#else
        close(_socketHandle);
#endif
        _socketHandle = 0;
        _state = Closed;
    }

    SDL_mutexV(_lock);
}

bool Socket::isOpen() const {
    bool ret;

    SDL_mutexP(_lock);
    ret = (_state == Bound || _state == Listening || _state == Connected);
    SDL_mutexV(_lock);

    return ret;
}
