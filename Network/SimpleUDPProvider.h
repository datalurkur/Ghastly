#ifndef SIMPLEUDPPROVIDER_H
#define SIMPLEUDPPROVIDER_H

#include <Network/ConnectionProvider.h>
#include <Network/UDPBuffer.h>

class SimpleUDPProvider: public ConnectionProvider {
public:
    SimpleUDPProvider(unsigned short localPort = 0);
    virtual ~SimpleUDPProvider();

    bool sendPacket(const Packet &packet);
    bool recvPacket(Packet &packet);

    unsigned short getLocalPort();

private:
    UDPBuffer *_buffer;
};

#endif