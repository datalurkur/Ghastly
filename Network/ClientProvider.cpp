#include <Network/ClientProvider.h>

bool ClientProvider::sendPacket(const Packet &packet) {
	TCPBuffer *buffer;
	ConnectionBufferMap::iterator itr = _buffers.find(packet.addr);
	if(itr == _buffers.end()) {
		buffer = new TCPBuffer(packet.addr);
		buffer->startBuffering();
		_buffers[packet.addr] = buffer;
	} else {
		buffer = (TCPBuffer*)itr->second;
	}
	return buffer->providePacket(packet);
}
