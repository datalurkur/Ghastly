#ifndef PACKETLISTENER_H
#define PACKETLISTENER_H

#include <Network/GhastlyProtocol.h>

typedef unsigned int NetworkID;
const NetworkID ID_NOT_FOUND = 0xffffffff;
const NetworkID SERVER_ID = 0;

class NetInterface;

class PacketListener {
public:
	PacketListener();
	virtual ~PacketListener();

	virtual void handlePacket(NetInterface *iface, NetworkID source, const GhastlyPacket *packet) = 0;

	void setInterface(NetInterface *iFace);
	bool sendPacket(NetworkID source, const GhastlyPacket &packet);

private:
	NetInterface *_interface;
};

#endif