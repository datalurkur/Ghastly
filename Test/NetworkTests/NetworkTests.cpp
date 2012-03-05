#include <Network/ListenSocket.h>
#include <Network/UDPBuffer.h>
#include <Network/TCPBuffer.h>
#include <Base/Assertion.h>
#include <Base/Log.h>

class SimpleConnectionListener: public SocketCreationListener {
public:
	SimpleConnectionListener(bool cleanup = true): _cleanup(cleanup) {}

	~SimpleConnectionListener() {
		if(_cleanup) {
			std::list<TCPSocket *>::iterator itr;
			for(itr = activeSockets.begin(); itr != activeSockets.end(); itr++) {
				delete *itr;
			}
		}
		activeSockets.clear();
	}

	bool onSocketCreation(const NetAddress &client, TCPSocket *socket) {
		activeSockets.push_back(socket);
		return true;
	}

	std::list<TCPSocket *> activeSockets;

private:
	bool _cleanup;
};

bool testUDP(bool blocking) {
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
    ret = socketA->send(stringA, strlen(stringA), addrB);
    ASSERT(ret);

    // Test socket receiving
    socketB->recv(dataBuffer, retSize, maxSize, retAddr);
    ASSERT(retAddr == addrA);
    ASSERT(retSize == (int)strlen(stringA));
    ASSERT(strncmp(dataBuffer, stringA, retSize) == 0);

    // Test socket sending from the other direction
    ret = socketB->send(stringB, strlen(stringB), addrA);
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

    return true;
}

bool testTCP(bool blocking) {
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
	ASSERT(connectionListener.activeSockets.size() > 0);

	buffer = (char*)calloc(bufferSize, sizeof(char));
	clientSocket->send(stringA, strlen(stringA));
	(*connectionListener.activeSockets.begin())->recv(buffer, size, bufferSize);
	ASSERT(size == strlen(stringA));
	ASSERT(strncmp(stringA, buffer, size) == 0);

	(*connectionListener.activeSockets.begin())->send(stringB, strlen(stringB));
	clientSocket->recv(buffer, size, bufferSize);
	ASSERT(size == strlen(stringB));
	ASSERT(strncmp(stringB, buffer, size) == 0);	
	free(buffer);

	delete clientSocket;
	delete listenSocket;

	return true;
}

bool testTCPBuffer(unsigned int maxPackets) {
	ListenSocket *listenSocket;
	TCPBuffer *clientBuffer, *serverBuffer;

	unsigned short clientPort,
				   serverPort;

	unsigned int maxCliPacketSize, maxSrvPacketSize, clientCounter, serverCounter, stringLength, c;

	Packet packet;

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
	ASSERT(connectionListener.activeSockets.size() > 0);

	serverBuffer = new TCPBuffer(*connectionListener.activeSockets.begin());

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
            clientBuffer->providePacket(Packet(serverAddr, data, stringLength));
            clientCounter++;
        }
        if(rand()%2==1) {
            stringLength = sprintf_s(data, maxSrvPacketSize, "%u", serverCounter);
            serverBuffer->providePacket(Packet(clientAddr, data, stringLength));
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

	serverBuffer->stopBuffering();
	clientBuffer->stopBuffering();

	delete clientBuffer;
	delete serverBuffer;
	delete listenSocket;

	return true;
}

bool testPacketBuffering(unsigned int maxPackets) {
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

    return true;
}

bool testUDPBuffer(unsigned int maxPackets) {
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

    return true;
}

int main(int argc, char *argv[]) {
    Log::Setup();
    Socket::InitializeSocketLayer();

    ASSERT(testUDP(true));
    ASSERT(testUDP(false));
	ASSERT(testTCP(true));
	ASSERT(testTCP(false));
    ASSERT(testPacketBuffering(2056));
    ASSERT(testUDPBuffer(2056));
	ASSERT(testTCPBuffer(2056));

    Socket::ShutdownSocketLayer();
	Log::Teardown();
    return 1;
}

