#ifndef UDPBUFFER_H
#define UDPBUFFER_H

#include <Network/ConnectionBuffer.h>
#include <Network/UDPSocket.h>

class UDPBuffer: public ConnectionBuffer {
public:
    UDPBuffer(unsigned short localPort = 0);
    virtual ~UDPBuffer();

    void doInboundBuffering();
    void doOutboundBuffering();

private:
    // Make sure the Socket* is properly cast so the correct functions get called
    inline UDPSocket* getSocket() { return (UDPSocket*)_socket; }
};

#endif
