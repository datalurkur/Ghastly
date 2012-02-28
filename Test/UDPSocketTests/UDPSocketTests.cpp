#include <Network/UDPSocket.h>

#include <string.h>

int main() {
    UDPSocket *socketA, *socketB;

    unsigned short portA = 5000,
                   portB = 6000;

    NetAddress addrA("127.0.0.1", portA, 4),
               addrB("127.0.0.1", portB, 4);

    const char *stringA = "Hello there, Socket B.",
               *stringB = "Well, well, if it isn't Socket A.";

    socketA = new UDPSocket(portA);
    socketB = new UDPSocket(portB);

    socketA->openSocket();
    socketB->openSocket();

    unsigned int maxSize = 1024, size;
    char buffer[1024];
    NetAddress tempAddr;

    socketA->send(addrB, stringA, sizeof(char)*strlen(stringA));
    socketB->recv(tempAddr, buffer, size, maxSize);
    ASSERT(tempAddr == addrA);
    ASSERT(strcmp(buffer, stringA) == 0);

    socketB->send(addrA, stringB, sizeof(char)*strlen(stringB));
    socketA->recv(tempAddr, buffer, size, maxSize);
    ASSERT(tempAddr == addrB);
    ASSERT(strcmp(buffer, stringB) == 0);

    socketA->closeSocket();
    socketB->closeSocket();

    delete socketA;
    delete socketB;

    printf("All tests completed successfully.");
}
