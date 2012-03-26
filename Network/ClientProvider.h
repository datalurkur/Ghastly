#ifndef CLIENTPROVIDER_H
#define CLIENTPROVIDER_H

#include <Network/MultiConnectionProvider.h>
#include <Network/TCPBuffer.h>

class ClientProvider: public MultiConnectionProvider {
public:
	~ClientProvider();

    bool sendPacket(const Packet &packet);
};

#endif
