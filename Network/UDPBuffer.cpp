#include <Network/UDPBuffer.h>
#include <Base/Log.h>

UDPBuffer::UDPBuffer(unsigned short localPort) {
    _socket = new UDPSocket();
    getSocket()->openSocket(localPort);
}

UDPBuffer::~UDPBuffer() {
    if(_socket) { delete getSocket(); }
}

void UDPBuffer::doInboundBuffering() {
    int size;
    NetAddress addr;

    Debug("Entering UDPBuffer inbound packet buffering loop");
    while(true) {
        SDL_LockMutex(_inboundQueueLock);

        // Get the next packet from the socket
        getSocket()->recv(_packetBuffer, size, _maxBufferSize, addr);

        if(size > 0) {
            // Update received stats
            _receivedPackets++;

            // Push the incoming packet onto the queue
            _inbound.push(Packet(addr, _packetBuffer, size));
            if(_inbound.size() > _maxBufferSize) {
                _inbound.pop();
                SDL_LockMutex(_outboundQueueLock);
                _droppedPackets++;
                SDL_UnlockMutex(_outboundQueueLock);
            } else {
                _inboundPackets++;
            }
        }
        SDL_UnlockMutex(_inboundQueueLock);
    }
}

void UDPBuffer::doOutboundBuffering() {
    bool ret;
    Packet packet;

    Debug("Entering UDP outbound packet buffering loop");
    while(true) {
        SDL_LockMutex(_outboundQueueLock);
        ret = _outbound.empty();
        if(!ret) {
            // Pop the next outgoing packet off the queue
            packet = _outbound.front();
            _outbound.pop();
            _outboundPackets--;

            // TODO - This is where we'd sleep the thread when throttling bandwidth

            // Send the next outgoing packet to the socket
            getSocket()->send(packet.data, packet.size, packet.addr);
            _sentPackets++;
        }
        SDL_UnlockMutex(_outboundQueueLock);
    }
}
