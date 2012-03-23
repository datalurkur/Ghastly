#include <Network/ClientProvider.h>

bool ClientProvider::sendPacket(const Packet &packet) {
	ConnectionBufferMap::iterator itr = _buffers.find(packet.addr);
	if(itr == _buffers.end()) {
		itr->second = new TCPBuffer(packet.addr);
	}
	return itr->second->providePacket(packet);
}
