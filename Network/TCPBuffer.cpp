#include <Network/TCPBuffer.h>
#include <Base/Assertion.h>

TCPBuffer::TCPBuffer(const NetAddress &dest, unsigned short localPort): _serializationBuffer(0) {
    _socket = new TCPSocket();
    getSocket()->connectSocket(dest, localPort);
}

TCPBuffer::TCPBuffer(TCPSocket *establishedSocket): _serializationBuffer(0) {
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
		packetSize, dataSize;
	char *dataBuffer;
	char *currentPacket;
    NetAddress addr;

	Debug("Waiting for TCPSocket to connect before starting inbound buffering");
	while(!getSocket()->isConnected()) { sleep(1); }

    Debug("Entering TCPBuffer inbound packet buffering loop");
    while(true) {
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
            _inbound.push(Packet(addr, dataBuffer, dataSize));
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
        SDL_LockMutex(_outboundQueueLock);
        if(!_outbound.empty()) {
            // Pop the next outgoing packet off the queue
            packet = _outbound.front();
            _outbound.pop();
            _outboundPackets--;

            // TODO - This is where we'd sleep the thread when throttling bandwidth

            // Send the next outgoing packet to the socket
			serializedSize = tcpSerialize(_serializationBuffer, packet.data, packet.size, _maxPacketSize);
            getSocket()->send(_serializationBuffer, serializedSize);
            _sentPackets++;
        }
        SDL_UnlockMutex(_outboundQueueLock);
    }
}

int TCPBuffer::tcpSerialize(char *dest, const char *src, int size, int maxSize) {
	int totalSize = sizeof(int) + size;

	ASSERT(totalSize <= maxSize);
	((int*)dest)[0] = totalSize;

	memcpy((void*)(dest + sizeof(int)), (void*)src, size);

	return totalSize;
}

int TCPBuffer::tcpDeserialize(const char *srcData, char **data, int &size) {
	size = ((int*)srcData)[0] - sizeof(int);
	*data = (char*)(srcData + sizeof(int));

	return sizeof(int) + size;
}
