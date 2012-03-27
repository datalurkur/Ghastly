#include <Network/ListenSocket.h>
#include <Network/UDPBuffer.h>
#include <Network/TCPBuffer.h>
#include <Network/ClientProvider.h>
#include <Network/ServerProvider.h>
#include <Network/SimpleUDPProvider.h>
#include <Network/GhastlyClient.h>
#include <Network/GhastlyServer.h>
#include <Base/Assertion.h>
#include <Base/Log.h>

class SimpleConnectionListener: public SocketCreationListener {
public:
	SimpleConnectionListener(bool cleanup = true): socket(0), _cleanup(cleanup) {}

	~SimpleConnectionListener() {
		if(_cleanup && socket) {
			delete socket;
		}
	}

	bool onSocketCreation(const NetAddress &client, TCPSocket *newSocket) {
		if(socket) {
			return false;
		} else {
			addr = client;
			socket = newSocket;
			return true;
		}
	}

	NetAddress addr;
	TCPSocket *socket;

private:
	bool _cleanup;
};

void testUDP(bool blocking) {
    unsigned short portA, portB;
    UDPSocket *socketA, *socketB;
    bool ret;

    Info("Running UDP socket tests");

    // The strings we'll send
    const char *stringA = "Greetings, Socket B",
         *stringB = "Well, well, if it isn't my old nemesis, Socket A";

    // Prepare for data buffering
    unsigned int maxSize = 1024;
    char *dataBuffer = (char*)calloc(maxSize,sizeof(char));
    int retSize;
    NetAddress retAddr;

    // Generate random ports
    portA = rand()%3000 + 3000;
    portB = rand()%3000 + 6000;

    // Prepare network addresses
    NetAddress addrA("127.0.0.1", portA), addrB("127.0.0.1", portB);

    // Test socket opening
    socketA = new UDPSocket(blocking);
    socketA->openSocket(portA);
    ASSERT(socketA->isOpen());

    socketB = new UDPSocket(blocking);
    socketB->openSocket(portB);
    ASSERT(socketB->isOpen());

    // Test socket sending
    ret = socketA->send(stringA, (unsigned int)strlen(stringA), addrB);
    ASSERT(ret);

    // Test socket receiving
    socketB->recv(dataBuffer, retSize, maxSize, retAddr);
    ASSERT(retAddr == addrA);
    ASSERT(retSize == (int)strlen(stringA));
    ASSERT(strncmp(dataBuffer, stringA, retSize) == 0);

    // Test socket sending from the other direction
    ret = socketB->send(stringB, (unsigned int)strlen(stringB), addrA);
    ASSERT(ret);

    // Test socket receiving from the other direction
    socketA->recv(dataBuffer, retSize, maxSize, retAddr);
    ASSERT(retAddr == addrB);
    ASSERT(retSize == (int)strlen(stringB));
    ASSERT(strncmp(dataBuffer, stringB, retSize) == 0);

    // Cleanup
    free(dataBuffer);

    delete socketA;
    delete socketB;
}

void testTCP(bool blocking) {
	TCPSocket *clientSocket;
	ListenSocket *listenSocket;

	unsigned short clientPort,
				   serverPort;

	const char *stringA = "I'm the client! I'm connected and stuff!",
			   *stringB = "Yeah, yeah, shut up already.";

	char *buffer;
	int size;
	unsigned int bufferSize = 1024;

	Info("Running TCP socket tests");

	SimpleConnectionListener connectionListener;
	listenSocket = new ListenSocket(&connectionListener);
	listenSocket->startListening();

	serverPort = listenSocket->getLocalPort();
	NetAddress serverAddr("127.0.0.1", serverPort);

	clientSocket = new TCPSocket(blocking);
	clientSocket->connectSocket(serverAddr);

	clientPort = clientSocket->getLocalPort();

	if(!blocking) {
		int secondsToWait = 5, c;
		bool connected = false;
		for(c = 0; c < secondsToWait; c++) {
			sleep(1);
			if(clientSocket->isConnected()) {
				connected = true;
				break;
			}
		}
		ASSERT(connected);
	} else {
		ASSERT(clientSocket->isConnected());
	}

	sleep(1);
	ASSERT(connectionListener.socket);

	buffer = (char*)calloc(bufferSize, sizeof(char));
	clientSocket->send(stringA, (unsigned int)strlen(stringA));
	(connectionListener.socket)->recv(buffer, size, bufferSize);
	ASSERT(size == (int)strlen(stringA));
	ASSERT(strncmp(stringA, buffer, size) == 0);

	(connectionListener.socket)->send(stringB, (unsigned int)strlen(stringB));
	clientSocket->recv(buffer, size, bufferSize);
	ASSERT(size == (int)strlen(stringB));
	ASSERT(strncmp(stringB, buffer, size) == 0);
	free(buffer);

	delete clientSocket;
	delete listenSocket;
}

void testTCPBuffer(unsigned int maxPackets) {
	ListenSocket *listenSocket;
	TCPBuffer *clientBuffer, *serverBuffer;

	unsigned short clientPort,
				   serverPort;

	unsigned int maxCliPacketSize, maxSrvPacketSize, clientCounter, serverCounter, stringLength, c;

	Info("Running TCP buffer tests");

	SimpleConnectionListener connectionListener(false);
	listenSocket = new ListenSocket(&connectionListener);
	listenSocket->startListening();

	serverPort = listenSocket->getLocalPort();
	NetAddress serverAddr("127.0.0.1", serverPort);

	clientBuffer = new TCPBuffer(serverAddr);
	clientPort = clientBuffer->getLocalPort();
	NetAddress clientAddr("127.0.0.1", clientPort);
	sleep(1);
	ASSERT(connectionListener.socket);

	serverBuffer = new TCPBuffer(connectionListener.addr, connectionListener.socket);

	serverBuffer->stopBuffering();
	clientBuffer->stopBuffering();
	serverBuffer->startBuffering();
	clientBuffer->startBuffering();

    maxCliPacketSize = clientBuffer->getMaxPacketSize();
    maxSrvPacketSize = serverBuffer->getMaxPacketSize();
    ASSERT(maxCliPacketSize == maxSrvPacketSize);

    clientCounter = 0;
    serverCounter = 0;
    char *data = (char*)calloc(maxSrvPacketSize, sizeof(char));
    for(c=0; c<maxPackets; c++) {
        if(rand()%2==1) {
            stringLength = sprintf_s(data, maxSrvPacketSize, "%u", clientCounter);
            ASSERT(clientBuffer->providePacket(Packet(serverAddr, data, stringLength)));
            clientCounter++;
        }
        if(rand()%2==1) {
            stringLength = sprintf_s(data, maxSrvPacketSize, "%u", serverCounter);
            ASSERT(serverBuffer->providePacket(Packet(clientAddr, data, stringLength)));
            serverCounter++;
        }
    }
    free(data);

	// Give the buffers some time to catch up
	sleep(1);
	clientBuffer->logStatistics();
	serverBuffer->logStatistics();

    data = (char*)calloc(maxSrvPacketSize, sizeof(char));
    for(c=0; c<clientCounter; c++) {
        Packet packet;
        stringLength = sprintf_s(data, maxSrvPacketSize, "%u", c);
        ASSERT(serverBuffer->consumePacket(packet));
        ASSERT(stringLength == packet.size);
        ASSERT(strncmp(data, packet.data, stringLength) == 0);
    }
    for(c=0; c<serverCounter; c++) {
        Packet packet;
        stringLength = sprintf_s(data, maxSrvPacketSize, "%u", c);
        ASSERT(clientBuffer->consumePacket(packet));
        ASSERT(stringLength == packet.size);
        ASSERT(strncmp(data, packet.data, stringLength) == 0);
    }
    free(data);

    unsigned int cliSentCounter = 0, srvSentCounter = 0, cliRecvCounter = 0, srvRecvCounter = 0;
    data = (char*)calloc(maxSrvPacketSize, sizeof(char));
    while(cliSentCounter < maxPackets && srvSentCounter < maxPackets && cliRecvCounter < maxPackets && srvRecvCounter < maxPackets) {
        if(rand()%2 == 0 && cliSentCounter < maxPackets) {
            stringLength = sprintf_s(data, maxSrvPacketSize, "%u", cliSentCounter);
            ASSERT(clientBuffer->providePacket(Packet(serverAddr, data, stringLength)));
            cliSentCounter++;
        }
        if(rand()%2 == 0 && srvSentCounter < maxPackets) {
            stringLength = sprintf_s(data, maxSrvPacketSize, "%u", srvSentCounter);
            ASSERT(serverBuffer->providePacket(Packet(serverAddr, data, stringLength)));
            srvSentCounter++;
        }

        if(rand()%2 == 0 && cliRecvCounter < maxPackets) {
            Packet packet;
            bool ret = clientBuffer->consumePacket(packet);

            if(cliRecvCounter < srvSentCounter) {
                if(ret) {
                    stringLength = sprintf_s(data, maxSrvPacketSize, "%u", cliRecvCounter);
                    ASSERT(stringLength == packet.size);
                    ASSERT(strncmp(data, packet.data, stringLength) == 0);
                    cliRecvCounter++;
                }
            } else {
                ASSERT(!ret);
            }
        }
        if(rand()%2 == 0 && srvRecvCounter < maxPackets) {
            Packet packet;
            bool ret = serverBuffer->consumePacket(packet);

            if(srvRecvCounter < cliSentCounter) {
                if(ret) {
                    stringLength = sprintf_s(data, maxSrvPacketSize, "%u", srvRecvCounter);
                    ASSERT(stringLength == packet.size);
                    ASSERT(strncmp(data, packet.data, stringLength) == 0);
                    srvRecvCounter++;
                }
            } else {
                ASSERT(!ret);
            }
        }
    }
    free(data);

	serverBuffer->stopBuffering();
	clientBuffer->stopBuffering();

	delete clientBuffer;
	delete serverBuffer;
	delete listenSocket;
}

void testPacketBuffering(unsigned int maxPackets) {
    std::queue<Packet> buffer;
    unsigned int c, size, bufferSize;
    Packet packet;

	Info("Running packet buffering tests");

	bufferSize = 1024;
    char *dataBuffer = (char*)calloc(bufferSize, sizeof(char));

    for(c=0; c<maxPackets; c++) {
        size = sprintf_s(dataBuffer, bufferSize, "%u", c);
        buffer.push(Packet(NetAddress("127.0.0.1", c), dataBuffer, size));
    }

    for(c=0; c<maxPackets; c++) {
        ASSERT(!buffer.empty());
        ASSERT(buffer.size() == maxPackets - c);
        packet = buffer.front();
        buffer.pop();
        size = sprintf_s(dataBuffer, bufferSize, "%u", c);
        ASSERT(size == packet.size);
        ASSERT(strncmp(packet.data, dataBuffer, size) == 0);
        ASSERT(packet.addr == NetAddress("127.0.0.1", c));
    }

	free(dataBuffer);
}

void testUDPBuffer(unsigned int maxPackets) {
    UDPBuffer *server, *client;
    unsigned short serverPort;
    unsigned int clientCounter, serverCounter, maxCliPacketSize, maxSrvPacketSize, c, stringLength;
    bool ret;

    Info("Running UDPBuffer tests");

    serverPort = rand()%3000+3000;

    server = new UDPBuffer(serverPort);
    client = new UDPBuffer();

    NetAddress serverAddr("127.0.0.1", serverPort),
               clientAddr("127.0.0.1", client->getLocalPort());

    server->startBuffering();
    client->startBuffering();

    maxCliPacketSize = client->getMaxPacketSize();
    maxSrvPacketSize = server->getMaxPacketSize();
    ASSERT(maxCliPacketSize == maxSrvPacketSize);

    clientCounter = 0;
    serverCounter = 0;
    char *data = (char*)calloc(maxSrvPacketSize, sizeof(char));
    for(c=0; c<maxPackets; c++) {
        if(rand()%2==1) {
            stringLength = sprintf_s(data, maxSrvPacketSize, "%u", clientCounter);
            client->providePacket(Packet(serverAddr, data, stringLength));
            clientCounter++;
        }
        if(rand()%2==1) {
            stringLength = sprintf_s(data, maxSrvPacketSize, "%u", serverCounter);
            server->providePacket(Packet(clientAddr, data, stringLength));
            serverCounter++;
        }
    }
    free(data);

	// Give the buffers some time to catch up
	sleep(1);
	client->logStatistics();
	server->logStatistics();

    data = (char*)calloc(maxSrvPacketSize, sizeof(char));
    for(c=0; c<clientCounter; c++) {
        Packet packet;
        stringLength = sprintf_s(data, maxSrvPacketSize, "%u", c);
        ret = server->consumePacket(packet);
        ASSERT(ret);
        ASSERT(stringLength == packet.size);
        ASSERT(strncmp(data, packet.data, stringLength) == 0);
    }
    for(c=0; c<serverCounter; c++) {
        Packet packet;
        stringLength = sprintf_s(data, maxSrvPacketSize, "%u", c);
        ret = client->consumePacket(packet);
        ASSERT(ret);
        ASSERT(stringLength == packet.size);
        ASSERT(strncmp(data, packet.data, stringLength) == 0);
    }
    free(data);

	client->stopBuffering();
	server->stopBuffering();

    delete client;
    delete server;
}

void testTCPConnectionProviders() {
    Info("Running TCPConnectionProvider tests");

    ClientProvider client;
    ServerProvider server;

    NetAddress serverAddr("127.0.0.1", server.getLocalPort());

    const char *messageA = "Hiya server",
               *messageB = "Why hello, client";
    Packet bufferPacket;

    ASSERT(client.sendPacket(Packet(serverAddr, messageA, strlen(messageA))));
    sleep(1);
    ASSERT(server.recvPacket(bufferPacket));
    ASSERT(strncmp(bufferPacket.data, messageA, bufferPacket.size) == 0);

    NetAddress clientAddr = bufferPacket.addr;
    ASSERT(server.sendPacket(Packet(clientAddr, messageB, strlen(messageB))));
    sleep(1);
    ASSERT(client.recvPacket(bufferPacket));
    ASSERT(strncmp(bufferPacket.data, messageB, bufferPacket.size) == 0);
}

void testUDPConnectionProviders() {
	Info("Running UDPConnectionProvider tests");

	SimpleUDPProvider client, server;

	NetAddress clientAddr("127.0.0.1", client.getLocalPort()),
			   serverAddr("127.0.0.1", server.getLocalPort());

	const char *messageA = "Hiya server",
			   *messageB = "Why hello, client";
	Packet bufferPacket;

	ASSERT(client.sendPacket(Packet(serverAddr, messageA, strlen(messageA))));
	sleep(1);
	ASSERT(server.recvPacket(bufferPacket));
	ASSERT(strncmp(bufferPacket.data, messageA, bufferPacket.size) == 0);

	ASSERT(server.sendPacket(Packet(clientAddr, messageB, strlen(messageB))));
	sleep(1);
	ASSERT(client.recvPacket(bufferPacket));
	ASSERT(strncmp(bufferPacket.data, messageB, bufferPacket.size) == 0);
}

void testGhastlyProtocolSetup() {
	Info("Running Ghastly protocol setup tests");

	GhastlyClient *client_1, *client_2;
	GhastlyServer *server;

	server = new GhastlyServer(1);
	client_1 = new GhastlyClient();
	client_2 = new GhastlyClient();

	NetAddress serverAddr("127.0.0.1", server->getLocalPort());

 	client_1->connect(serverAddr);
	sleep(1);
	server->update(1);
	sleep(1);
	client_1->update(1);
	ASSERT(client_1->getState() == GhastlyClient::READY);

	client_2->connect(serverAddr);
	sleep(1);
	server->update(1);
	sleep(1);
	client_2->update(1);
	ASSERT(client_2->getState() == GhastlyClient::NOT_CONNECTED);

	client_1->disconnect();
	sleep(1);
	server->update(1);
	ASSERT(client_1->getState() == GhastlyClient::NOT_CONNECTED);
	delete client_1;

	client_2->connect(serverAddr);
	sleep(1);
	server->update(1);
	sleep(1);
	client_2->update(1);
	ASSERT(client_2->getState() == GhastlyClient::READY);

	delete server;
	sleep(1);
	client_2->update(1);
	ASSERT(client_2->getState() == GhastlyClient::NOT_CONNECTED);

	delete client_2;
}

int main(int argc, char *argv[]) {
    Log::Setup();
    Socket::InitializeSocketLayer();

    testUDP(true);
    testUDP(false);
	testTCP(true);
	testTCP(false);
    testPacketBuffering(2^16);
    testUDPBuffer(2^16);
	testTCPBuffer(2^16);
	testTCPConnectionProviders();
	testUDPConnectionProviders();
	testGhastlyProtocolSetup();

    Socket::ShutdownSocketLayer();
	Log::Teardown();
    return 1;
}

