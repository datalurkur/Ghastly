#include <Network/UDPSocket.h>
#include <Base/Log.h>

UDPSocket::UDPSocket(unsigned short port, bool blocking): _port(port), _blocking(blocking) {
    openSocket();
}

UDPSocket::~UDPSocket() {
    if(_open) { closeSocket(); }
}

bool UDPSocket::openSocket() {
    sockaddr_in addr;

    // Lock the mutex
    SDL_mutexP(_lock);

    // Obviously we don't need to open the socket if it's already open
    if(_open) {
        SDL_mutexV(_lock);
        return true;
    }

    // Create the socket
    _socketHandle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(_socketHandle <= 0) {
        Error("Failed to create UDP socket.");
        SDL_mutexV(_lock);
        return false;
    }

    // Bind the socket
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(_port);
    if(bind(_socketHandle, (const sockaddr*)&addr, sizeof(sockaddr_in)) < 0) {
        Error("Failed to bind UDP socket to port " << _port);
        SDL_mutexV(_lock);
        return false;
    }

    // If we got a random port, figure out what port we were assigned
    if(_port == 0) {
        sockaddr_in addr;
        socklen_t addrSize = sizeof(addr);
        if(getsockname(_socketHandle, (sockaddr*)&addr, &addrSize) == 0 && addr.sin_family == AF_INET && sizeof(addr) == addrSize) {
            _port = ntohs(addr.sin_port);
            Debug("UDPSocket assigned port " << _port);
        } else {
            Error("Failed to assign port to UDPSocket");
            SDL_mutexV(_lock);
            return false;
        }
    }

    // Set the socket nonblocking if required
    if(!_blocking) {
#if SYS_PLATFORM == PLATFORM_WIN32
		DWORD nonBlock = 1;
        if(ioctlsocket(_socketHandle, FIONBIO, &nonBlock) != 0) {
#else
        int nonBlock = 1;
        if(fcntl(_socketHandle, F_SETFL, O_NONBLOCK, nonBlock) == -1) {
#endif
            Error("Failed to set socket as non-blocking.");
            SDL_mutexV(_lock);
            return false;
        }
    }

    _open = true;

    SDL_mutexV(_lock);
	return true;
}

void UDPSocket::closeSocket() {
    SDL_mutexP(_lock);

#if SYS_PLATFORM == PLATFORM_WIN32
	closesocket(_socketHandle);
#else
	close(_socketHandle);
#endif
	_socketHandle = 0;
    _open = false;

    SDL_mutexV(_lock);
}

unsigned short UDPSocket::getPort() const { return _port; }

bool UDPSocket::send(const NetAddress &dst, const char *data, unsigned int size) {
    unsigned int bytesSent;

    ASSERT(_open);
    SDL_mutexP(_lock);

    ASSERT(_open);
    bytesSent = sendto(_socketHandle, data, size, 0, dst.getSockAddr(), dst.getSockAddrSize());

    SDL_mutexV(_lock);

	if(bytesSent != size) {
		Error("Failed to send UDP packet: Sent " << bytesSent << " bytes out of " << size);
		return false;
	}
	return true;
}

void UDPSocket::recv(NetAddress &src, char *data, int &size, unsigned int maxSize) {
    // This is where, normally, we'd split between IPv4 and IPv6
	// For simplicity, and because no-one uses IPv6 yet, we're doing IPv4
	sockaddr_in sndAddr;
	socklen_t sndAddrSize = sizeof(sndAddr);

    SDL_mutexP(_lock);

	ASSERT(_open);

	// WARNING: Any packets received that are larger than maxSize are SILENTLY discarded
	// I know, right? How ridiculous is that? Thanks, OBAMA.
	size = recvfrom(_socketHandle, data, maxSize, 0, (sockaddr*)&sndAddr, &sndAddrSize);
	src = NetAddress(&sndAddr);

    SDL_mutexV(_lock);
}
