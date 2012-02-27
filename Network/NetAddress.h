#ifndef NETADDRESS_H
#define NETADDRESS_H

#include <Network/Socket.h>

// Currently does not support IPv6
class NetAddress {
public:
    NetAddress();
    NetAddress(const char *addr, unsigned short port, unsigned char ipVersion);
	NetAddress(const sockaddr_in *addrData);
	NetAddress(const sockaddr_in6 *addrData);
    ~NetAddress();

    const sockaddr *getSockAddr() const;
	unsigned int getSockAddrSize() const;

	inline void operator=(const NetAddress& rhs) {
		_ipv4Addr = rhs._ipv4Addr;
		_ipv6Addr = rhs._ipv6Addr;
		_ipVersion = rhs._ipVersion;
	}

protected:
    sockaddr_in   _ipv4Addr;
    sockaddr_in6  _ipv6Addr;
    unsigned char _ipVersion;
};

#endif
