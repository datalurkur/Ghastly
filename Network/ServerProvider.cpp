#include <Network/ServerProvider.h>
#include <Base/Assertion.h>

ServerProvider::ServerProvider(unsigned short localPort) {
	_listenSocket = new ListenSocket(this);
	_listenSocket->startListening(localPort);
}

ServerProvider::~ServerProvider() {
	delete _listenSocket;
}

bool ServerProvider::sendPacket(const Packet &packet) {
	TCPBufferMap::iterator itr = _buffers.find(packet.addr);

	if(itr == _buffers.end()) { return false; }

	return itr->second->providePacket(packet);
}

bool ServerProvider::recvPacket(Packet &packet) {
    ASSERT(0);
    // Basically, we need a way to prioritize which buffers get read from
	TCPBufferMap::iterator itr = _buffers.begin();
	return itr->second->consumePacket(packet);
}

unsigned short ServerProvider::getLocalPort() {
	return _listenSocket->getLocalPort();
}

bool ServerProvider::onSocketCreation(const NetAddress &client, TCPSocket *socket) {
	TCPBufferMap::iterator itr = _buffers.find(client);

	if(itr == _buffers.end()) {
		// This connection already exists, kill the old one and replace it with this one
		delete itr->second;
		_buffers.erase(itr);
	}

	_buffers[client] = new TCPBuffer(socket);
	return true;
}
