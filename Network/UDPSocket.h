#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <Network/Socket.h>
#include <Network/NetAddress.h>

// IPv6 support is...well, nonexistent. YOU implement an IP-version agnostic socket. Go ahead. I'll wait.
class UDPSocket: public Socket {
public:
    UDPSocket(unsigned short port, bool blocking = false);
    virtual ~UDPSocket();

    bool openSocket();
    void closeSocket();

	void setMaxPacketSize(unsigned int size);
	unsigned int getMaxPacketSize() const;

    bool send(const NetAddress &dst, const char *data, unsigned int size);
    void recv(NetAddress &src, char *data, unsigned int &size, unsigned int maxSize);

private:
	unsigned short _port;
    bool _blocking;
};

#endif
