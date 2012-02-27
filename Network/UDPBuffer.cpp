#include <Network/UDPBuffer.h>

UDPBuffer::UDPBuffer(): _sendSocket(0) {
}

UDPBuffer::~UDPBuffer() {
}

void UDPBuffer::addSocket(UDPSocket *socket) {
    UDPSocketList::iterator itr;
    for(itr = _sockets.begin(); itr != _sockets.end(); itr++) {
        if(*itr == socket) { return; }
    }
    _sockets.push_back(socket);
}

void UDPBuffer::setSendSocket(UDPSocket *socket) {
    _sendSocket = socket;
}

void UDPBuffer::fetchPackets() {
    UDPSocketList::iterator itr;

    for(itr = _sockets.begin(); itr != _sockets.end(); itr++) {
        while(true) {
            unsigned int size = 0;
            char *data = 0;
            NetAddress addr;

            (*itr)->recv(addr, data, size);

            if(size <= 0) { break; }

            _incoming.push(Packet(addr, data, size));
        }
    }
}

bool UDPBuffer::pushPacket(const NetAddress &dst, const char *data, unsigned int size) {
    ASSERT(_sendSocket);
    return _sendSocket->send(dst, data, size);
}

