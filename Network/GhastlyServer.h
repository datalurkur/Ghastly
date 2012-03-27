#ifndef GHASTLYSERVER_H
#define GHASTLYSERVER_H

#include <Network/GhastlyHost.h>
#include <Network/SimpleUDPProvider.h>
#include <Base/IndexPool.h>
#include <Base/Timestamp.h>

struct GhastlyHostInfo {
	NetAddress addr;
	HostID id;
	clock_t lastReceived;
	double latency;

	GhastlyHostInfo();
	GhastlyHostInfo(const GhastlyHostInfo &other);
	GhastlyHostInfo(const NetAddress &a, HostID i);

	void operator=(const GhastlyHostInfo &other);
	void copy(const GhastlyHostInfo &other);
};

#define DEFAULT_MAX_CLIENTS	256

class GhastlyServer: public GhastlyHost, public SimpleUDPProvider {
public:
	GhastlyServer(unsigned int maxClients = DEFAULT_MAX_CLIENTS);
	~GhastlyServer();

	void update(int elapsed);
	void onPacketReceive(const Packet &packet);

private:
	unsigned int _maxClients;

	IndexPool *_idPool;

	typedef std::map<NetAddress,HostID> IDMap;
	IDMap _idMap;

	typedef std::map<HostID,GhastlyHostInfo> HostMap;
	HostMap _hostMap;
};

#endif