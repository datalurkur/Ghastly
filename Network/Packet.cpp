#include <Network/Packet.h>

Packet::Packet(): size(0), data(0) {}

Packet::Packet(const NetAddress &a, const char *d, unsigned int s): addr(a), size(s) {
    data = (char*)calloc(size, sizeof(char));
    memcpy(data, d, size);
}

Packet::~Packet() {
    free(data);
}

const Packet& Packet::operator=(const Packet &rhs) {
    if(data) { free(data); }
    addr = rhs.addr;
    size = rhs.size;
    data = (char*)calloc(size, sizeof(char));
    memcpy(data, rhs.data, size);
    return *this;
}
