#include <Network/GhastlyServer.h>
#include <Base/Assertion.h>

GhastlyServer::GhastlyServer(unsigned int maxClients): GhastlyHost(ID_SERVER), _maxClients(maxClients) {
	_clientPool = new ResourcePool<HostID>(maxClients);
}

GhastlyServer::~GhastlyServer() {
	// Send disconnect messages to all the clients before tearing down
	ASSERT(0);
}

void GhastlyServer::update(int elapsed) {
	dispatchIncomingPackets();
}

void GhastlyServer::onPacketReceive(const Packet &packet) {
	Payload *payload = (Payload*)packet.data;
	switch(payload->type) {
	case IDRequestType:
		
		break;
	}
}