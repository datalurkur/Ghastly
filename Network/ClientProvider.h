#ifndef CLIENTPROVIDER_H
#define CLIENTPROVIDER_H

#include <Network/ConnectionProvider.h>
#include <Network/TCPBuffer.h>

class ClientProvider: public ConnectionProvider {
public:
    bool sendPacket(const Packet &packet);
	bool recvPacket(Packet &packet);

private:
	TCPBufferMap _buffers;
};

#endif