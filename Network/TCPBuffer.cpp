#include <Network/TCPBuffer.h>

TCPBuffer::TCPBuffer(unsigned short localPort, const NetAddress &dest) {
    _socket = new TCPSocket();
    _socket->connectSocket(localPort, dest);
}

TCPBuffer::~TCPBuffer() {
    if(_socket) { delete _socket; }
}

void TCPBuffer::doBuffering() {
}
