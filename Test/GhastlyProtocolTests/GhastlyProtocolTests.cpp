#include <Network/UDPInterface.h>
#include <Base/Log.h>

int main(int argc, char *argv[]) {
	unsigned short serverPort = 1234,
				   clientPort = 2345;
	UDPInterface *server, *client;

	Log::EnableAllChannels(); 

	ASSERT(Socket::InitializeSocketLayer());

	server = new UDPInterface(serverPort);
	client = new UDPInterface(clientPort, NetAddress("127.0.0.1", serverPort));

	for(int c=0; c<10; c++) {
		server->processPackets();
		client->processPackets();
		sleep(1);
	}

	delete client;
	delete server;

	Socket::ShutdownSocketLayer();

	Info("Test completed successfully.");

    return 0;
}
