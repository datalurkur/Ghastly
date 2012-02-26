#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <Network/Socket.h>
#include <Network/NetAddress.h>

#define DEFAULT_MAX_PACKET_SIZE 1024

// IPv6 support is...well, nonexistent. YOU implement an IP-version agnostic socket. Go ahead. I'll wait.
class UDPSocket: public Socket {
public:
    UDPSocket(unsigned short port, bool blocking);
    virtual ~UDPSocket();

    bool open();
    void close();

	void setMaxPacketSize(unsigned int size);
	unsigned int getMaxPacketSize() const;

	// Returns true if the packet was sent successfully
    bool send(const NetAddress &dest, const char *data, unsigned int size);
    void recv(NetAddress &src, char *data, unsigned int &size);

private:
	unsigned int _maxPacketSize;
	unsigned short _port;
    bool _blocking;
};

#endif
