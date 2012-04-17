#include <Network/TCPBuffer.h>
#include <Base/Assertion.h>

TCPBuffer::TCPBuffer(const NetAddress &dest, unsigned short localPort): _serializationBuffer(0), _dest(dest) {
    _socket = new TCPSocket();
    getSocket()->connectSocket(dest, localPort);
}

TCPBuffer::TCPBuffer(const NetAddress &dest, TCPSocket *establishedSocket): _serializationBuffer(0), _dest(dest) {
    _socket = establishedSocket;
}

TCPBuffer::~TCPBuffer() {
     if(_socket) { delete getSocket(); }
}

void TCPBuffer::startBuffering() {
    if(!_serializationBuffer) {
        _serializationBuffer = (char*)calloc(_maxPacketSize, sizeof(char));
    }
    ConnectionBuffer::startBuffering();
}

void TCPBuffer::stopBuffering() {
    ConnectionBuffer::stopBuffering();
    if(_serializationBuffer) {
        free(_serializationBuffer);
    }
}

void TCPBuffer::doInboundBuffering() {
    int totalBufferSize, currentOffset,
        packetSize;
    unsigned int dataSize;
    char *dataBuffer;
    char *currentPacket;

    Debug("Waiting for TCPSocket to connect before starting inbound buffering");
    while(!getSocket()->isConnected()) { sleep(1); }

    Debug("Entering TCPBuffer inbound packet buffering loop");
    while(true) {
        SDL_LockMutex(_inboundLock);
        if(_inboundShouldDie) {
            SDL_UnlockMutex(_inboundLock);
            break;
        } else {
            SDL_UnlockMutex(_inboundLock);
        }
        
        SDL_LockMutex(_inboundQueueLock);

        // Get the next packet from the socket
        getSocket()->recv(_packetBuffer, totalBufferSize, _maxBufferSize);
        currentOffset = 0;
        dataBuffer = 0;
        while(currentOffset < totalBufferSize) {
            currentPacket = _packetBuffer + currentOffset;
            packetSize = tcpDeserialize(currentPacket, &dataBuffer, dataSize);

            // Update received stats
            _receivedPackets++;

            // Push the incoming packet onto the queue
            _inbound.push(Packet(_dest, dataBuffer, dataSize));
            if(_inbound.size() > _maxBufferSize) {
                _inbound.pop();
                SDL_LockMutex(_outboundQueueLock);
                _droppedPackets++;
                SDL_UnlockMutex(_outboundQueueLock);
            } else {
                _inboundPackets++;
            }

            currentOffset += packetSize;
        }
        SDL_UnlockMutex(_inboundQueueLock);
    }
}

void TCPBuffer::doOutboundBuffering() {
    Packet packet;
    int serializedSize;

    Debug("Waiting for TCPSocket to connect before starting outbound buffering");
    while(!getSocket()->isConnected()) { sleep(1); }

    Debug("Entering TCPBuffer outbound packet buffering loop");
    while(true) {
        SDL_LockMutex(_outboundLock);
        if(_outboundShouldDie) {
            SDL_UnlockMutex(_outboundLock);
            break;
        } else {
            SDL_UnlockMutex(_outboundLock);
        }
        
        SDL_LockMutex(_outboundQueueLock);
        if(!_outbound.empty()) {
            // Pop the next outgoing packet off the queue
            packet = _outbound.front();
            _outbound.pop();
            _outboundPackets--;

            // TODO - This is where we'd sleep the thread when throttling bandwidth

            // Send the next outgoing packet to the socket
            serializedSize = tcpSerialize(_serializationBuffer, packet.data, (unsigned int)packet.size, _maxPacketSize);
            getSocket()->send(_serializationBuffer, serializedSize);
            _sentPackets++;
        }
        SDL_UnlockMutex(_outboundQueueLock);
    }
}

int TCPBuffer::tcpSerialize(char *dest, const char *src, unsigned int size, unsigned int maxSize) {
    uint32_t totalSize = sizeof(uint32_t) + size;

    ASSERT(totalSize <= maxSize);
    ((uint32_t*)dest)[0] = totalSize;

    memcpy((void*)(dest + sizeof(uint32_t)), (void*)src, size);

    return totalSize;
}

int TCPBuffer::tcpDeserialize(const char *srcData, char **data, unsigned int &size) {
    size = ((uint32_t*)srcData)[0] - sizeof(uint32_t);
    *data = (char*)(srcData + sizeof(uint32_t));

    return sizeof(int) + size;
}
