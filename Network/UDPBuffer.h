#ifndef UDPBUFFER_H
#define UDPBUFFER_H

#include <Network/PacketBuffer.h>
#include <Network/UDPSocket.h>

// This class is pretty braindead / pointless, since most of this functional layer is designed for simplifying TCP
class UDPBuffer: public PacketBuffer {
public:
    UDPBuffer();
    virtual ~UDPBuffer();

    void addSocket(UDPSocket *socket);
    void setSendSocket(UDPSocket *socket);

    void fetchPackets();
    bool pushPacket(const NetAddress &dst, const char *data, unsigned int size);

private:
    typedef std::list<UDPSocket*> UDPSocketList;
    UDPSocketList _sockets;
    UDPSocket *_sendSocket;
};

#endif
