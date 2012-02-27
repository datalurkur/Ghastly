#ifndef PACKETBUFFER_H
#define PACKETBUFFER_H

#include <Base/Base.h>
#include <Network/NetAddress.h>

struct Packet {
    Packet(NetAddress a, char *d, unsigned int s): addr(a), data(d), size(s) {}
    Packet(const Packet &other): addr(other.addr), data(other.data), size(other.size) {}

    NetAddress addr;
    char *data;
    unsigned int size;
};

class PacketBuffer {
public:
    PacketBuffer();
    virtual ~PacketBuffer();

    virtual void fetchPackets() = 0;

    void popPacket(NetAddress &src, char *data, unsigned int &size);
    virtual bool pushPacket(const NetAddress &dst, const char *data, unsigned int size) = 0;

protected:
    typedef std::queue<Packet> PacketQueue;
    PacketQueue _incoming;
};

#endif
