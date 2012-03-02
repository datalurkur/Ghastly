#include <Network/Packet.h>
#include <Base/Assertion.h>
#include <Base/Log.h>

Packet::Packet(): size(0), data(0) {
}

Packet::Packet(const Packet &other): size(0), data(0) {
    duplicate(other);
}

Packet::Packet(const NetAddress &a, const char *d, unsigned int s): addr(a), size(s) {
    data = (char*)calloc(size, sizeof(char));
    memcpy(data, d, size);
}

Packet::~Packet() {
    if(data) {
        free(data);
    }
}

const Packet& Packet::operator=(const Packet &rhs) {
    duplicate(rhs);
    return *this;
}

void Packet::duplicate(const Packet &other) {
    if(data) { free(data); }
    addr = other.addr;
    size = other.size;
    data = (char*)calloc(size, sizeof(char));
    memcpy(data, other.data, size);
    ASSERT(data != other.data);
}
