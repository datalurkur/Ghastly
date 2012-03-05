#ifndef SERVERPROVIDER_H
#define SERVERPROVIDER_H

#include <Network/ConnectionProvider.h>
#include <Network/TCPBuffer.h>
#include <Network/ListenSocket.h>

class ServerProvider: public ConnectionProvider, public SocketCreationListener {
public:
	ServerProvider(unsigned short localPort = 0);
	virtual ~ServerProvider();

    bool sendPacket(const Packet &packet);
	bool recvPacket(Packet &packet);

	unsigned short getLocalPort();

	bool onSocketCreation(const NetAddress &client, TCPSocket *socket);

private:
	TCPBufferMap _buffers;
	ListenSocket *_listenSocket;
};

#endif