#include <Network/UDPInterface.h>

UDPInterface::UDPInterface(UDPSocket *socket): _socket(socket) {
}

UDPInterface::~UDPInterface() {
}

void UDPInterface::recv(unsigned int &id, char *data, unsigned int &size) {
    NetAddress sender;
    _socket->recv(sender, data, size, _maxPacketSize);
    id = getNetworkID(sender);
}

bool UDPInterface::send(unsigned int id, const char *data, unsigned int size) {
    NetAddress rcpt;

    // Break up the packet into multiples based on max payload size
    ASSERT(size <= _maxPacketSize); // Has yet to be implemented

    // Obviously we can't send data to an address we don't know
    ASSERT(netIDKnown(id));
    rcpt = getNetworkAddress(id);

    return _socket->send(rcpt, data, size);
}
