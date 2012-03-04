#include <Network/ConnectionProvider.h>
#include <Base/Log.h>

ConnectionProvider::ConnectionProvider(): _fastBuffer(0), _listenSocket(0) {
}

ConnectionProvider::~ConnectionProvider() {
	TCPBufferMap::iterator itr;
	for(itr = _tcpBufferMap.begin(); itr != _tcpBufferMap.end(); itr ++) {
		delete itr->second;
	}
	_tcpBufferMap.clear();
}

void ConnectionProvider::sendFastPacket(const Packet &packet) {
    if(!_fastBuffer) {
        Error("Fast buffer not ready.");
        return;
    }

    _fastBuffer->providePacket(packet);
}

/*
void ConnectionProvider::sendReliablePacket(const Packet &packet) {
}
*/

void ConnectionProvider::setupFastBuffer(unsigned short port) {
    if(_fastBuffer) {
        Warn("Fast buffer already primed.");
        return;
    }

    _fastBuffer = new UDPBuffer(port);
}

void ConnectionProvider::setupReliableBuffer(unsigned short port) {
	if(_listenSocket) {
		Warn("Reliable buffer already primed.");
		return;
	}

	_listenSocket = new ListenSocket(this);
	_listenSocket->startListening(port);
}

bool ConnectionProvider::onSocketCreation(const NetAddress &client, TCPSocket *socket) {
	TCPBufferMap::iterator existingClient;

	existingClient = _tcpBufferMap.find(client);
	if(existingClient != _tcpBufferMap.end()) {
		Warn("Client already has an active connection, tearing down the old connection and reestablishing.");
		// We already have a connection established with this client, tear down the old connection before accepting the new one
		delete existingClient->second;
		_tcpBufferMap.erase(existingClient);
	}

	// Create a new TCPBuffer using the existing socket
	TCPBuffer *newBuffer = new TCPBuffer(socket);

	// Insert the new TCPBuffer into the buffer map
	_tcpBufferMap[client] = newBuffer;

	Info("Accepted connection from " << client);
	return true;
}