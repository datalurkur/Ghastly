#include <Network/PacketBuffer.h>

PacketBuffer::PacketBuffer() {
}

PacketBuffer::~PacketBuffer() {
}

void PacketBuffer::popPacket(NetAddress &src, char *data, unsigned int &size) {
    Packet next(_incoming.front());

    src = next.addr;
    data = next.data;
    size = next.size;

    _incoming.pop();
}
