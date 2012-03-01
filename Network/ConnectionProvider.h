#ifndef CONNECTIONPROVIDER_H
#define CONNECTIONPROVIDER_H

#include <Network/UDPBuffer.h>
#include <Network/TCPBuffer.h>

// TODO - Code reliable connections in the form of TCP
class ConnectionProvider {
public:
    ConnectionProvider();
    virtual ~ConnectionProvider();

    void sendFastPacket(const Packet &packet);
    //void sendReliablePacket(const Packet &packet);

    void setupFastBuffer(unsigned short port = 0);

private:
    UDPBuffer *_fastBuffer;

    //typedef std::map<NetAddress,TCPBuffer*> TCPBufferMap;
    //TCPBufferMap _tcpBufferMap;
};

#endif
