#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <Network/Socket.h>
#include <Network/NetAddress.h>

// IPv6 support is...well, nonexistent. YOU implement an IP-version agnostic socket. Go ahead. I'll wait.
class UDPSocket: public Socket {
public:
    UDPSocket(bool blocking = false);
    virtual ~UDPSocket();

    bool openSocket(unsigned short localPort = 0);

    bool send(const char *data, unsigned int size, const NetAddress &addr);
    void recv(char *data, int &size, unsigned int maxSize, NetAddress &addr);

private:
	unsigned short _port;
};

#endif
