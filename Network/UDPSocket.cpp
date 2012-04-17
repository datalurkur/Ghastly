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

bool UDPSocket::send(const char *data, unsigned int size, const NetAddress &addr) {
    return Socket::send(data, size, addr.getSockAddr(), addr.getSockAddrSize());
}

void UDPSocket::recv(char *data, int &size, unsigned int maxSize, NetAddress &addr) {
    sockaddr_in addrData;
    int addrSize = sizeof(addrData);
    Socket::recv(data, size, maxSize, (sockaddr*)&addrData, addrSize);
    addr = NetAddress(&addrData);
}