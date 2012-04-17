#ifndef MULTICONNECTIONPROVIDER_H
#define MULTICONNECTIONPROVIDER_H

#include <Network/ConnectionBuffer.h>
#include <Network/ConnectionProvider.h>

class MultiConnectionProvider: public ConnectionProvider {
public:
    bool recvPacket(Packet &packet);

protected:
    void getAndPrioritizePackets();

protected:
    std::priority_queue<Packet> _prioritizedPackets;
    ConnectionBufferMap _buffers;
};

#endif
