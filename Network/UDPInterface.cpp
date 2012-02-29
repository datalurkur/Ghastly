#include <Network/UDPInterface.h>
#include <Base/Log.h>

UDPInterface::UDPInterface(unsigned short port):
	NetInterface() {
	setup(port);
}

UDPInterface::UDPInterface(unsigned short port, const NetAddress &server):
	NetInterface(server) {
	setup(port);
}

UDPInterface::~UDPInterface() {
	teardown();
}

void UDPInterface::setup(unsigned short port) {
	Info("Setting up UDPInterface");
	_socket = new UDPSocket(port);
	_socket->openSocket();
	NetInterface::setup();
}

void UDPInterface::teardown() {
	Info("Tearing down UDPInterface");
	NetInterface::teardown();
	_socket->closeSocket();
	delete _socket;
}

void UDPInterface::processPackets() {
    NetAddress sender;
	char *buffer;
	int size;

	ASSERT(_socket);

	// Allocate memory for our packet buffer
	buffer = (char*)calloc(_maxPacketSize, sizeof(char));

	while(true) {
		NetworkID id;
		GhastlyPacket *packet;

		_socket->recv(sender, buffer, size, _maxPacketSize);
		if(size <= 0) {
			break;
		}
		packet = (GhastlyPacket*)buffer;
		ASSERT(packet->size == size);

		id = getNetworkID(sender);

		processPacket(id, sender, packet, (id == ID_NOT_FOUND));
	}

	free(buffer);
}

bool UDPInterface::send(const NetAddress &addr, const GhastlyPacket &packet) {
	ASSERT(_socket);
    return _socket->send(addr, (char*)&packet, packet.size);
}
