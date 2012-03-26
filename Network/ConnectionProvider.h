#ifndef CONNECTIONPROVIDER_H
#define CONNECTIONPROVIDER_H

#include <Network/Packet.h>

class PacketListener {
public:
	virtual void onPacketReceive(const Packet &packet) = 0;
};

class ConnectionProvider {
public:
	ConnectionProvider();

    virtual bool sendPacket(const Packet &packet) = 0;
	virtual bool recvPacket(Packet &packet) = 0;

	unsigned int dispatchIncomingPackets(unsigned int maxPackets = 0);
	void registerIncomingPacketListener(PacketListener *listener);

private:
	PacketListener *_listener;
};

#endif
