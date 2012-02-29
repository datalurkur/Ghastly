#include <Network/NetInterface.h>

PacketListener::PacketListener(): _interface(0) {
}

PacketListener::~PacketListener() {
}

void PacketListener::setInterface(NetInterface *iFace) {
	// A packet listener should really not be registered with more than one interface
	ASSERT(!_interface);
	_interface = iFace;
}

bool PacketListener::sendPacket(NetworkID source, const GhastlyPacket &packet) {
	return _interface->send(source, packet);
}