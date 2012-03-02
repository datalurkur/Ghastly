#include <Network/UDPBuffer.h>
#include <Base/Assertion.h>
#include <Base/Log.h>

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
    socketA = new UDPSocket(portA, blocking);
    ASSERT(socketA->isOpen());

    socketB = new UDPSocket(portB, blocking);
    ASSERT(socketB->isOpen());

    // Test socket sending
    ret = socketA->send(addrB, stringA, strlen(stringA));
    ASSERT(ret);

    // Test socket receiving
    socketB->recv(retAddr, dataBuffer, retSize, maxSize);
    ASSERT(retAddr == addrA);
    ASSERT(retSize == (int)strlen(stringA));
    ASSERT(strncmp(dataBuffer, stringA, retSize) == 0);

    // Test socket sending from the other direction
    ret = socketB->send(addrA, stringB, strlen(stringB));
    ASSERT(ret);

    // Test socket receiving from the other direction
    socketA->recv(retAddr, dataBuffer, retSize, maxSize);
    ASSERT(retAddr == addrB);
    ASSERT(retSize == (int)strlen(stringB));
    ASSERT(strncmp(dataBuffer, stringB, retSize) == 0);

    // Cleanup
    free(dataBuffer);

    delete socketA;
    delete socketB;

    return true;
}

bool testPacketBuffering(unsigned int maxPackets) {
    std::queue<Packet> buffer;
    unsigned int c, size, bufferSize;
    Packet packet;

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
    Log::EnableAllChannels();
    Socket::InitializeSocketLayer();

    ASSERT(testUDP(true));
    ASSERT(testUDP(false));
    ASSERT(testPacketBuffering(2056));
    ASSERT(testUDPBuffer(2056));

    Socket::ShutdownSocketLayer();
    return 1;
}

