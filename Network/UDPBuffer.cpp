#include <Network/UDPBuffer.h>
#include <Base/Log.h>

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

void UDPBuffer::doInboundBuffering() {
    int size;
    NetAddress addr;

    Debug("Entering UDPBuffer buffering loop");
    while(true) {
        // Lock the packet buffer while it's in use
        Debug("Acquiring buffer lock");
        SDL_mutexP(_bufferLock);

        // Get the next packet from the socket
        Debug("Receiving data");
        _socket->recv(addr, _packetBuffer, size, _maxBufferSize);

        if(size > 0) {
            Debug("Data received acquiring queue lock");
            SDL_mutexP(_inboundQueueLock);
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
            SDL_mutexV(_inboundQueueLock);
        } else {
            Debug("No data to receive");
        }
        // Unlock the packet buffer for potential resizing
        SDL_mutexV(_bufferLock);
    }
}

void UDPBuffer::doOutboundBuffering() {
    bool ret;
    Packet packet;

    while(true) {
        SDL_mutexP(_outboundQueueLock);
        ret = _outbound.empty();
        if(!ret) {
            // Pop the next outgoing packet off the queue
            packet = _outbound.front();
            _outbound.pop();

            // Update sent stats
            _outboundPackets--;
            _sentPackets++;
        }
        SDL_mutexV(_outboundQueueLock);

        // TODO - This is where we'd sleep the thread when throttling bandwidth

        if(!ret) {
            Debug("Sending packet from queue");
            // Send the next outgoing packet to the socket
            _socket->send(packet.addr, packet.data, packet.size);
        } else {
            Debug("No packets in queue to send");
        }
    }
}

unsigned short UDPBuffer::getLocalPort() const {
    if(_socket) {
        return _socket->getPort();
    } else {
        return 0;
    }
}
