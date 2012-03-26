#ifndef GHASTLYSERVER_H
#define GHASTLYSERVER_H

#include <Network/GhastlyHost.h>
#include <Network/SimpleUDPProvider.h>
#include <Base/ResourcePool.h>

struct GhastlyHostInfo {
	NetAddress addr;
	HostID id;
	clock_t lastReceived;
	double latency;
};

class GhastlyServer: public GhastlyHost, public SimpleUDPProvider {
public:
	GhastlyServer(unsigned int maxClients);
	~GhastlyServer();

	void update(int elapsed);
	void onPacketReceive(const Packet &packet);

private:
	unsigned int _maxClients;

	ResourcePool<HostID> *_clientPool;
};

#endif