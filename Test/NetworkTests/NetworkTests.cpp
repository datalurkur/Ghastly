#include <Network/UDPSocket.h>

bool testUDP(bool blocking) {
    unsigned short portA, portB;
    UDPSocket *socketA, *socketB;
    bool ret;

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

int main() {
    ASSERT(testUDP(true));
    ASSERT(testUDP(false));
    return 1;
}

