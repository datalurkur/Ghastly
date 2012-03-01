#include <Network/ConnectionProvider.h>
#include <Base/Log.h>

ConnectionProvider::ConnectionProvider() {
}

ConnectionProvider::~ConnectionProvider() {
}

void ConnectionProvider::sendFastPacket(const Packet &packet) {
    if(!_fastBuffer) {
        Error("Fast buffer not ready.");
        return;
    }

    _fastBuffer->providePacket(packet);
}

/*
void ConnectionProvider::sendReliablePacket(const Packet &packet) {
}
*/

void ConnectionProvider::setupFastBuffer(unsigned short port) {
    if(_fastBuffer) {
        Warn("Fast buffer already primed.");
        return;
    }

    _fastBuffer = new UDPBuffer(port);
}
