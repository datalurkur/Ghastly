#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <Network/Socket.h>
#include <Network/NetAddress.h>

class TCPSocket: public Socket {
public:
    TCPSocket(unsigned short port, bool blocking = false);
    virtual ~TCPSocket();

    bool openSocket();
    void closeSocket();
};

#endif
