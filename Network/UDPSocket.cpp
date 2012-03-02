#include <Network/UDPSocket.h>
#include <Base/Assertion.h>
#include <Base/Log.h>

UDPSocket::UDPSocket(bool blocking): Socket(blocking) {
}

UDPSocket::~UDPSocket() {
}

bool UDPSocket::openSocket(unsigned short localPort) {
    if(!createSocket(SOCK_DGRAM, IPPROTO_UDP)) { return false; }
    if(!bindSocket(localPort)) { return false; }

	return true;
}

unsigned short UDPSocket::getLocalPort() const {
    unsigned short ret;
    sockaddr_in addr;
    socklen_t addrSize = sizeof(addr);

    ASSERT(isOpen());

    SDL_mutexP(_lock);
    if(getsockname(_socketHandle, (sockaddr*)&addr, &addrSize) == 0 && addr.sin_family == AF_INET && sizeof(addr) == addrSize) {
        ret = ntohs(addr.sin_port);
    } else {
        Error("Failed to get port for UDPSocket");
        ret = 0;
    }
    SDL_mutexV(_lock);

    return ret;
}

bool UDPSocket::send(const NetAddress &dst, const char *data, unsigned int size) {
    unsigned int bytesSent;

    ASSERT(isOpen());

    SDL_mutexP(_lock);
    bytesSent = sendto(_socketHandle, data, size, 0, dst.getSockAddr(), dst.getSockAddrSize());
    SDL_mutexV(_lock);

	if(bytesSent != size) {
		Error("Failed to send UDP packet: Sent " << bytesSent << " bytes out of " << size);
		return false;
	} else {
        return true;
	}
}

void UDPSocket::recv(NetAddress &src, char *data, int &size, unsigned int maxSize) {
    // This is where, normally, we'd split between IPv4 and IPv6
	// For simplicity, and because no-one uses IPv6 yet, we're doing IPv4
	sockaddr_in sndAddr;
	socklen_t sndAddrSize = sizeof(sndAddr);

    ASSERT(isOpen());

    SDL_mutexP(_lock);
	// WARNING: Any packets received that are larger than maxSize are SILENTLY discarded
	// I know, right? How ridiculous is that? Thanks, OBAMA.
	size = recvfrom(_socketHandle, data, maxSize, 0, (sockaddr*)&sndAddr, &sndAddrSize);
	src = NetAddress(&sndAddr);
    SDL_mutexV(_lock);
}
