#include <Network/SimpleUDPProvider.h>

SimpleUDPProvider::SimpleUDPProvider(unsigned short localPort) {
	_buffer = new UDPBuffer(localPort);
}

SimpleUDPProvider::~SimpleUDPProvider() {
	delete _buffer;
}

bool SimpleUDPProvider::sendPacket(const Packet &packet) {
	return _buffer->providePacket(packet);
}

bool SimpleUDPProvider::recvPacket(Packet &packet) {
	return _buffer->consumePacket(packet);
}

unsigned short SimpleUDPProvider::getLocalPort() {
	return _buffer->getLocalPort();
}