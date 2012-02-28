#ifndef NETADDRESS_H
#define NETADDRESS_H

#include <Network/Socket.h>

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

    inline bool operator==(const NetAddress& rhs) {
        return (_ipVersion == rhs._ipVersion) &&
               (    (_ipVersion == 4 &&
                        (_ipv4Addr.sin_addr.s_addr == rhs._ipv4Addr.sin_addr.s_addr &&
                         _ipv4Addr.sin_port        == rhs._ipv4Addr.sin_port)
                    ) ||
                    (_ipVersion == 6 &&
                        (_ipv6Addr.sin6_addr.s6_addr == rhs._ipv6Addr.sin6_addr.s6_addr &&
                         _ipv6Addr.sin6_port         == rhs._ipv6Addr.sin6_port)
                    )
               );
    }

protected:
    sockaddr_in   _ipv4Addr;
    sockaddr_in6  _ipv6Addr;
    unsigned char _ipVersion;
};

#endif
