#ifndef CONNECTIONPROVIDER_H
#define CONNECTIONPROVIDER_H

#include <Network/UDPBuffer.h>
#include <Network/TCPBuffer.h>
#include <Network/ListenSocket.h>

// TODO - Code reliable connections in the form of TCP
class ConnectionProvider: public SocketCreationListener {
public:
    ConnectionProvider();
    virtual ~ConnectionProvider();

    void sendFastPacket(const Packet &packet);
    //void sendReliablePacket(const Packet &packet);

    void setupFastBuffer(unsigned short port = 0);
	void setupReliableBuffer(unsigned short port = 0);

	bool onSocketCreation(const NetAddress &client, TCPSocket *socket);

private:
    UDPBuffer *_fastBuffer;

	ListenSocket *_listenSocket;

    typedef std::map<NetAddress,TCPBuffer*> TCPBufferMap;
    TCPBufferMap _tcpBufferMap;
};

#endif
