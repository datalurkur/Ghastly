#include <Network/TCPSocket.h>
#include <Base/Assertion.h>
#include <Base/Log.h>

TCPSocket::TCPSocket(bool blocking): Socket(blocking) {
}

TCPSocket::~TCPSocket() {
}

bool TCPSocket::connectSocket(unsigned short localPort, const NetAddress &dest) {
    if(!createSocket(SOCK_STREAM, IPPROTO_TCP)) { return false; }
    if(!bindSocket(localPort)) { return false; }

	return true;
}

bool TCPSocket::listenSocket(unsigned short localPort) {
    if(!createSocket(SOCK_STREAM, IPPROTO_TCP)) { return false; }
    if(!bindSocket(localPort)) { return false; }

	return true;
}
