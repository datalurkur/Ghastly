#include <Network/ConnectionProvider.h>

#include <Base/Assertion.h>

ConnectionProvider::ConnectionProvider(): _listener() {}

unsigned int ConnectionProvider::dispatchIncomingPackets(unsigned int maxPackets) {
	unsigned int c;
	Packet packet;

	for(c=0; c<maxPackets; c++) {
		if(recvPacket(packet)) {
			_listener->onPacketReceive(packet);
		} else {
			break;
		}
	}

	return c;
}

void ConnectionProvider::registerIncomingPacketListener(PacketListener *listener) {
	ASSERT(!_listener);
	_listener = listener;
}