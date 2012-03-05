#include <Network/ClientProvider.h>

bool ClientProvider::sendPacket(const Packet &packet) {
	TCPBufferMap::iterator itr = _buffers.find(packet.addr);
	if(itr == _buffers.end()) {
		itr->second = new TCPBuffer(packet.addr);
	}
	return itr->second->providePacket(packet);
}

bool ClientProvider::recvPacket(Packet &packet) {
	TCPBufferMap::iterator itr = _buffers.find(packet.addr);
	if(itr == _buffers.end()) {
		itr->second = new TCPBuffer(packet.addr);
	}
	return itr->second->consumePacket(packet);
}