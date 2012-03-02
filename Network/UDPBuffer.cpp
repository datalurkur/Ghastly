#include <Network/UDPBuffer.h>
#include <Base/Log.h>

UDPBuffer::UDPBuffer() {
    _socket = new UDPSocket();
    _socket->openSocket();
}

UDPBuffer::UDPBuffer(unsigned short localPort) {
    _socket = new UDPSocket();
    _socket->openSocket(localPort);
}

UDPBuffer::~UDPBuffer() {
    if(_socket) { delete _socket; }
}

void UDPBuffer::doInboundBuffering() {
    int size;
    NetAddress addr;

    Debug("Entering UDPBuffer inbound packet buffering loop");
    while(true) {
        SDL_mutexP(_inboundQueueLock);

        // Get the next packet from the socket
        _socket->recv(addr, _packetBuffer, size, _maxBufferSize);

        if(size > 0) {
            // Update received stats
            _receivedPackets++;

            // Push the incoming packet onto the queue
            _inbound.push(Packet(addr, _packetBuffer, size));
            if(_inbound.size() > _maxBufferSize) {
                _inbound.pop();
                SDL_mutexP(_outboundQueueLock);
                _droppedPackets++;
                SDL_mutexV(_outboundQueueLock);
            } else {
                _inboundPackets++;
            }
        }
        SDL_mutexV(_inboundQueueLock);
    }
}

void UDPBuffer::doOutboundBuffering() {
    bool ret;
    Packet packet;

	Debug("Entering UDP outbound packet buffering loop");
    while(true) {
        SDL_mutexP(_outboundQueueLock);
        ret = _outbound.empty();
        if(!ret) {
            // Pop the next outgoing packet off the queue
            packet = _outbound.front();
            _outbound.pop();
            _outboundPackets--;

            // TODO - This is where we'd sleep the thread when throttling bandwidth

            // Send the next outgoing packet to the socket
            _socket->send(packet.addr, packet.data, packet.size);
            _sentPackets++;
        }
        SDL_mutexV(_outboundQueueLock);
    }
}

unsigned short UDPBuffer::getLocalPort() const {
    if(_socket) {
        return _socket->getLocalPort();
    } else {
        return 0;
    }
}
