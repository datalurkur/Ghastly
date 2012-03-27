#include <Network/ConnectionProvider.h>

#include <Base/Assertion.h>

ConnectionProvider::ConnectionProvider(): _listener() {}

unsigned int ConnectionProvider::dispatchIncomingPackets(unsigned int maxPackets) {
	unsigned int c;
	Packet packet;

	if(!_listener) { return 0; }

	c = 0;
	while(recvPacket(packet) && (maxPackets == 0 || maxPackets > c++)) {
		_listener->onPacketReceive(packet);
	}

	return c;
}

void ConnectionProvider::registerIncomingPacketListener(PacketListener *listener) {
	ASSERT(!_listener);
	_listener = listener;
}