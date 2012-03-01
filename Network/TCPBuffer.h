#ifndef TCPBUFFER_H
#define TCPBUFFER_H

#include <Network/ConnectionBuffer.h>
#include <Network/TCPSocket.h>

class TCPBuffer: public ConnectionBuffer {
public:
    TCPBuffer();
    TCPBuffer(unsigned short localPort);
    virtual ~TCPBuffer();

    void doBuffering();

private:
    TCPSocket *_socket;
};

#endif
