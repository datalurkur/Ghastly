#include <Network/UDPBuffer.h>

// Since we're threaded, make this a blocking socket for simplicity
//  (and also so our threads don't eat up tons of CPU time, gadflying for data)

UDPBuffer::UDPBuffer() {
    _socket = new UDPSocket(0, true);
}

UDPBuffer::UDPBuffer(unsigned short localPort) {
    _socket = new UDPSocket(localPort, true);
}

UDPBuffer::~UDPBuffer() {
    if(_socket) { delete _socket; }
}

void UDPBuffer::doBuffering() {
    int size;
    NetAddress addr;
    Packet packet;
    bool ret;

    while(true) {
        // Lock the packet buffer while it's in use
        SDL_mutexP(_bufferLock);

        // Get the next packet from the socket
        _socket->recv(addr, _packetBuffer, size, _maxBufferSize);

        if(size > 0) {
            SDL_mutexP(_lock);
            // Update received stats
            _receivedPackets++;

            // Push the incoming packet onto the queue
            _inbound.push(Packet(addr, _packetBuffer, size));
            if(_inbound.size() > _maxBufferSize) {
                _inbound.pop();
                _droppedPackets++;
            } else {
                _inboundPackets++;
            }
            SDL_mutexV(_lock);
        }
        // Unlock the packet buffer for potential resizing
        SDL_mutexV(_bufferLock);

        SDL_mutexP(_lock);
        ret = _outbound.empty();
        if(!ret) {
            // Pop the next outgoing packet off the queue
            packet = _outbound.front();
            _outbound.pop();

            // Update sent stats
            _outboundPackets--;
            _sentPackets++;
        }
        SDL_mutexV(_lock);

        // TODO - This is where we'd sleep the thread when throttling bandwidth

        // Send the next outgoing packet to the socket
        _socket->send(packet.addr, packet.data, packet.size);
    }
}
