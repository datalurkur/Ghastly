#ifndef UDPBUFFER_H
#define UDPBUFFER_H

#include <Network/ConnectionBuffer.h>
#include <Network/UDPSocket.h>

class UDPBuffer: public ConnectionBuffer {
public:
    UDPBuffer();
    UDPBuffer(unsigned short localPort);
    virtual ~UDPBuffer();

    void doBuffering();

private:
    UDPSocket *_socket;
};

#endif
