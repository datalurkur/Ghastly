#include <Network/UDPSocket.h>
#include <Base/Log.h>

int main(int argc, char *argv[]) {
    UDPSocket *socketA, *socketB;

    unsigned short portA = 5000,
                   portB = 6000;

    NetAddress addrA("127.0.0.1", portA, 4),
               addrB("127.0.0.1", portB, 4);

    const char *stringA = "Hello there, Socket B.",
               *stringB = "Well, well, if it isn't Socket A.";

	Log::EnableAllChannels();
	ASSERT(Socket::InitializeSocketLayer());

    socketA = new UDPSocket(portA);
    socketB = new UDPSocket(portB);

    socketA->openSocket();
	ASSERT(socketA->isOpen());
    socketB->openSocket();
	ASSERT(socketB->isOpen());

    unsigned int maxSize = 1024;
	int size;
    char *buffer = (char*)calloc(maxSize, sizeof(char));
    NetAddress tempAddr;

	bool success;

    success = socketA->send(addrB, stringA, sizeof(char)*strlen(stringA));
	ASSERT(success);

    socketB->recv(tempAddr, buffer, size, maxSize);
	ASSERT(size >= 0);
    ASSERT(tempAddr == addrA);
    ASSERT(strcmp(buffer, stringA) == 0);

    success = socketB->send(addrA, stringB, sizeof(char)*strlen(stringB));
	ASSERT(success);

    socketA->recv(tempAddr, buffer, size, maxSize);
	ASSERT(size >= 0);
    ASSERT(tempAddr == addrB);
    ASSERT(strcmp(buffer, stringB) == 0);

    socketA->closeSocket();
	ASSERT(!socketA->isOpen());
    socketB->closeSocket();
	ASSERT(!socketB->isOpen());

    delete socketA;
    delete socketB;

	Socket::ShutdownSocketLayer();

	free(buffer);

	Info("Test completed successfully.");

	return 0;
}
