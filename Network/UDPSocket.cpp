#include <Network/UDPSocket.h>
#include <Base/Log.h>

UDPSocket::UDPSocket(unsigned short port, bool blocking = false): _port(port), _blocking(blocking) {
}

UDPSocket::~UDPSocket() {
    if(_open) { closeSocket(); }
}

bool UDPSocket::openSocket() {
    sockaddr_in addr;

    // Create the socket
    _socketHandle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(_socketHandle <= 0) {
        Error("Failed to create UDP socket.");
        return false;
    }

    // Bind the socket
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(_port);
    if(bind(_socketHandle, (const sockaddr*)&addr, sizeof(sockaddr_in)) < 0) {
        Error("Failed to bind UDP socket to port " << _port);
        return false;
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
            return false;
        }
    }

    _open = true;
	return true;
}

void UDPSocket::closeSocket() {
#if SYS_PLATFORM == PLATFORM_WIN32
	closesocket(_socketHandle);
#else
	close(_socketHandle);
#endif
	_socketHandle = 0;
    _open = false;
}

void UDPSocket::setMaxPacketSize(unsigned int size) {
	_maxPacketSize = size;
}

unsigned int UDPSocket::getMaxPacketSize() const {
	return _maxPacketSize;
}

bool UDPSocket::send(const NetAddress &dest, const char *data, unsigned int size) {
    unsigned int bytesSent;

    ASSERT(_open);

	if(size > _maxPacketSize) {
		Warn("Packet size exceeds receive window.  This could mean something is amiss.");
	}

    bytesSent = sendto(_socketHandle, data, size, 0, dest.getSockAddr(), dest.getSockAddrSize());
	if(bytesSent != size) {
		Error("Failed to send UDP packet: Sent " << bytesSent << " bytes out of " << size);
		return false;
	}

	return true;
}

void UDPSocket::recv(NetAddress &src, char *data, unsigned int &size) {
    // This is where, normally, we'd split between IPv4 and IPv6
	// For simplicity, and because no-one uses IPv6 yet, we're doing IPv4
	sockaddr_in sndAddr;
	socklen_t sndAddrSize = sizeof(sndAddr);

	ASSERT(_open);

	// WARNING: Any packets received that are larger than _maxPacketSize are SILENTLY discarded
	// I know, right? How ridiculous is that? Thanks, OBAMA.
	data = (char*)calloc(_maxPacketSize, sizeof(char));
	size = recvfrom(_socketHandle, data, _maxPacketSize, 0, (sockaddr*)&sndAddr, &sndAddrSize);
	src = NetAddress(&sndAddr);
}
