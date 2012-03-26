#ifndef GHASTLYCLIENT_H
#define GHASTLYCLIENT_H

#include <Network/GhastlyHost.h>
#include <Network/SimpleUDPProvider.h>

typedef unsigned char ClientState;

class GhastlyClient: public GhastlyHost, public SimpleUDPProvider {
public:
	enum {
		NOT_CONNECTED = 0,
		AWAITING_ID,
		AWAITING_DATA,
		READY
	};

public:
	GhastlyClient();
	~GhastlyClient();

	ClientState getState() const;
	void update(int elapsed);
	void onPacketReceive(const Packet &packet);

	void connect(const NetAddress &addr);
	void disconnect();

private:
	ClientState _state;
	NetAddress _server;
};

#endif