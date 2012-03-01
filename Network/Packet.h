#ifndef PACKET_H
#define PACKET_H

#include <Network/NetAddress.h>

struct Packet {
    NetAddress addr;
    unsigned int size;
    char *data;

    Packet();
    Packet(const NetAddress &a, const char *d, unsigned int s);
    ~Packet();

    const Packet& operator=(const Packet &rhs);
};

#endif
