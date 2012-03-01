#include <Network/TCPBuffer.h>

TCPBuffer::TCPBuffer() {
    _socket = new TCPSocket(0, true);
}

TCPBuffer::TCPBuffer(unsigned short localPort) {
    _socket = new TCPSocket(localPort, true);
}

TCPBuffer::~TCPBuffer() {
    if(_socket) { delete _socket; }
}

void TCPBuffer::doBuffering() {
}
