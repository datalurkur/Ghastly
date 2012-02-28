#include <Network/UDPInterface.h>

UDPInterface::UDPInterface(UDPSocket *socket): _socket(socket) {
}

UDPInterface::~UDPInterface() {
}

void UDPInterface::recv(unsigned int &id, char *data, unsigned int &size) {
    Address sender;
    _socket->recv(&sender, data, size);
    id = getNetworkID(&sender);
}

bool UDPInterface::send(unsigned int id, const char *data, unsigned int size) {
}
