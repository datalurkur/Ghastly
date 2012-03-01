#include <Network/UDPBuffer.h>
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
    char *dataBuffer = (char*)calloc(maxSize,sizeof(char)),
         *cmpBuffer;
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

    // Make sure strings compare
    cmpBuffer = (char*)calloc(retSize, sizeof(char));
    memcpy(cmpBuffer, dataBuffer, retSize);
    ASSERT(strcmp(cmpBuffer, stringA) == 0);
    free(cmpBuffer);

    // Test socket sending from the other direction
    ret = socketB->send(addrA, stringB, strlen(stringB));
    ASSERT(ret);

    // Test socket receiving from the other direction
    socketA->recv(retAddr, dataBuffer, retSize, maxSize);
    ASSERT(retAddr == addrB);
    ASSERT(retSize == (int)strlen(stringB));

    // Compare strings once again
    cmpBuffer = (char*)calloc(retSize, sizeof(char));
    memcpy(cmpBuffer, dataBuffer, retSize);
    ASSERT(strcmp(cmpBuffer, stringB) == 0);
    free(cmpBuffer);

    // Cleanup
    free(dataBuffer);

    delete socketA;
    delete socketB;

    return true;
}

bool testPacketBuffering(unsigned int maxPackets) {
    std::queue<Packet> buffer;
    unsigned int c, size;
    Packet packet;
    char dataBuffer[1024];

    for(c=0; c<maxPackets; c++) {
        size = sprintf(dataBuffer, "%u", c);
        Info("Pushing " << dataBuffer);
        buffer.push(Packet(NetAddress("127.0.0.1", c), dataBuffer, size));
        packet = buffer.front();
        buffer.pop();
        Info("Verify: " << packet.data);
    }
/*
    for(c=0; c<maxPackets; c++) {
        ASSERT(!buffer.empty());
        ASSERT(buffer.size() == maxPackets - c);
        packet = buffer.front();
        Info("Popping " << packet.data << " sized " << packet.size);
        buffer.pop_front();
        size = sprintf(dataBuffer, "%u", c);
        //ASSERT(size == packet.size);
        //ASSERT(strcmp(packet.data, dataBuffer) == 0);
        //ASSERT(packet.addr == NetAddress("127.0.0.1", c));
    }
*/
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
            stringLength = sprintf(data, "%u", clientCounter);
            client->providePacket(Packet(serverAddr, data, stringLength));
            clientCounter++;
        }
        if(rand()%2==1) {
            stringLength = sprintf(data, "%u", serverCounter);
            server->providePacket(Packet(clientAddr, data, stringLength));
            serverCounter++;
        }
    }
    free(data);

    data = (char*)calloc(maxSrvPacketSize, sizeof(char));
    for(c=0; c<clientCounter; c++) {
        Packet packet;
        stringLength = sprintf(data, "%u", clientCounter);
        ret = server->consumePacket(packet);
        ASSERT(ret);
        ASSERT(stringLength == packet.size);
        ASSERT(strcmp(data, packet.data) == 0);
    }
    for(c=0; c<serverCounter; c++) {
        Packet packet;
        stringLength = sprintf(data, "%u", serverCounter);
        ret = client->consumePacket(packet);
        ASSERT(ret);
        ASSERT(stringLength == packet.size);
        ASSERT(strcmp(data, packet.data) == 0);
    }
    free(data);

    delete client;
    delete server;

    return true;
}

int main() {
    Log::EnableAllChannels();
    Socket::InitializeSocketLayer();
    ASSERT(testUDP(true));
    ASSERT(testUDP(false));
    ASSERT(testPacketBuffering(200));
    //ASSERT(testUDPBuffer(1000));
    Socket::ShutdownSocketLayer();
    return 1;
}

