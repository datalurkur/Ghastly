#ifndef SERVERPROVIDER_H
#define SERVERPROVIDER_H

#include <Network/MultiConnectionProvider.h>
#include <Network/TCPBuffer.h>
#include <Network/ListenSocket.h>

class ServerProvider: public MultiConnectionProvider, public SocketCreationListener {
public:
	ServerProvider(unsigned short localPort = 0);
	virtual ~ServerProvider();

    bool sendPacket(const Packet &packet);

	unsigned short getLocalPort();

	bool onSocketCreation(const NetAddress &client, TCPSocket *socket);

private:
	ListenSocket *_listenSocket;
};

#endif
