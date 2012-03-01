#ifndef UDPBUFFER_H
#define UDPBUFFER_H

#include <Network/ConnectionBuffer.h>
#include <Network/UDPSocket.h>

class UDPBuffer;

class UDPBuffer: public ConnectionBuffer {
public:
    UDPBuffer();
    UDPBuffer(unsigned short localPort);
    virtual ~UDPBuffer();

    void doInboundBuffering();
    void doOutboundBuffering();

    unsigned short getLocalPort() const;

private:
    UDPSocket *_socket;
};

#endif
