#include <Network/Packet.h>
#include <Base/Assertion.h>
#include <Base/Log.h>

Packet::Packet(): size(0), data(0) {
}

Packet::Packet(const Packet &other): size(0), data(0) {
    duplicate(other);
}

Packet::Packet(const NetAddress &a, const char *d, unsigned int s): addr(a), size(s) {
    data = (char*)calloc(s, sizeof(char));
    memcpy(data, d, s);
    //Debug("Allocating " << (void*)data << " in constructor");
}

Packet::~Packet() {
    if(data) {
        //Debug("Freeing " << (void*)data << " in destructor");
        free(data);
        data = 0;
    }
}

const Packet& Packet::operator=(const Packet &rhs) {
    duplicate(rhs);
    return *this;
}

void Packet::duplicate(const Packet &other) {
    if(data) {
        //Debug("Freeing " << (void*)data << " in duplicate");
        free(data);
        data = 0;
    }
    addr = other.addr;
    size = other.size;
    data = (char*)calloc(size, sizeof(char));
    memcpy(data, other.data, size);
    //Debug("Allocating " << (void*)data << " in duplicate");
    ASSERT(data != other.data);
}
