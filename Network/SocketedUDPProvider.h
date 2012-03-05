#ifndef SOCKETEDUDPPROVIDER_H
#define SOCKETEDUDPPROVIDER_H

#include <Network/ConnectionProvider.h>
#include <Network/UDPBuffer.h>

// The SocketedUDPProvider is designed to mimic the behavior of a ServerProvider over UDP instead of TCP
// This allows individual ConnectionBuffers to be created for each unique client address, which allows greater control over per-client bandwidth

class SocketedUDPProvider: public ConnectionProvider {
public:
	SocketedUDPProvider(unsigned short port = 0);
	virtual ~SocketedUDPProvider();
};

#endif