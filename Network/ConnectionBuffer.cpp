#include <Network/ConnectionBuffer.h>
#include <Base/Assertion.h>
#include <Base/Log.h>

int InvokeInboundConnectionBufferThreadFunction(void *params) {
    ConnectionBuffer *cBuffer = (ConnectionBuffer*)params;
    cBuffer->doInboundBuffering();
    return 1;
}

int InvokeOutboundConnectionBufferThreadFunction(void *params) {
    ConnectionBuffer *cBuffer = (ConnectionBuffer*)params;
    cBuffer->doOutboundBuffering();
    return 1;
}

unsigned int ConnectionBuffer::DefaultMaxBufferSize = 5096;

unsigned int ConnectionBuffer::DefaultMaxPacketSize = 1024;

ConnectionBuffer::ConnectionBuffer():
    _socket(0), _inboundThread(0), _outboundThread(0),
    _packetBuffer(0), _maxBufferSize(DefaultMaxBufferSize), _maxPacketSize(DefaultMaxPacketSize),
    _droppedPackets(0), _receivedPackets(0), _sentPackets(0), _inboundPackets(0), _outboundPackets(0)
{
}

ConnectionBuffer::~ConnectionBuffer() {
    ASSERT(!_inboundThread && !_outboundThread);
}

void ConnectionBuffer::startBuffering() {
    if(!_inboundThread) {
        _inboundQueueLock  = SDL_CreateMutex();
        _inboundLock = SDL_CreateMutex();
        _inboundShouldDie = false;

        _packetBuffer = (char*)calloc(_maxPacketSize, sizeof(char));
        _inboundThread = SDL_CreateThread(InvokeInboundConnectionBufferThreadFunction, "InboundConnectionBufferThread", (void*)this);
    }
    if(!_outboundThread) {
        _outboundQueueLock = SDL_CreateMutex();
        _outboundLock = SDL_CreateMutex();
        _outboundShouldDie = false;

        _outboundThread = SDL_CreateThread(InvokeOutboundConnectionBufferThreadFunction, "OutboundConnectionBufferThread", (void*)this);
    }
}

void ConnectionBuffer::stopBuffering() {
    if(_inboundThread) {
        int status;
    
        SDL_LockMutex(_inboundLock);
        _inboundShouldDie = true;
        SDL_UnlockMutex(_inboundLock);
        SDL_WaitThread(_inboundThread, &status);
        _inboundThread = 0;

        free(_packetBuffer);
        _packetBuffer = 0;
        
        SDL_DestroyMutex(_inboundQueueLock);
        SDL_DestroyMutex(_inboundLock);
    }
    if(_outboundThread) {
        int status;

        SDL_LockMutex(_outboundLock);
        _outboundShouldDie = true;
        SDL_UnlockMutex(_outboundLock);
        SDL_WaitThread(_outboundThread, &status);
        _outboundThread = 0;
        
        SDL_DestroyMutex(_outboundQueueLock);
        SDL_DestroyMutex(_outboundLock);
    }
}

void ConnectionBuffer::setMaxBufferSize(unsigned int maxPackets) {
    _maxBufferSize = maxPackets;
}

unsigned int ConnectionBuffer::getMaxBufferSize() {
    return _maxBufferSize;
}

void ConnectionBuffer::setMaxPacketSize(unsigned int maxSize) {
    _maxPacketSize = maxSize;
    SDL_LockMutex(_inboundQueueLock);
    _packetBuffer = (char*)realloc(_packetBuffer, _maxPacketSize*sizeof(char));
    SDL_UnlockMutex(_inboundQueueLock);
}

unsigned int ConnectionBuffer::getMaxPacketSize() {
    return _maxPacketSize;
}

bool ConnectionBuffer::providePacket(const Packet &packet) {
    bool ret;

    SDL_LockMutex(_outboundQueueLock);
    _outbound.push(packet);
    if(_outbound.size() > _maxBufferSize) {
        _outbound.pop();
        SDL_LockMutex(_inboundQueueLock);
        _droppedPackets++;
        SDL_UnlockMutex(_inboundQueueLock);
        ret = false;
    } else {
        _outboundPackets++;
        ret = true;
    }
    SDL_UnlockMutex(_outboundQueueLock);

    return ret;
}

bool ConnectionBuffer::consumePacket(Packet &packet) {
    bool ret;

    SDL_LockMutex(_inboundQueueLock);
    if(_inbound.empty()) { ret = false; }
    else {
        packet = _inbound.front();
        _inbound.pop();
        _inboundPackets--;
        ret = true;
    }
    SDL_UnlockMutex(_inboundQueueLock);

    return ret;
}

unsigned short ConnectionBuffer::getLocalPort() const {
    if(_socket) {
        return _socket->getLocalPort();
    } else {
        return 0;
    }
}

void ConnectionBuffer::logStatistics() {
    SDL_LockMutex(_inboundQueueLock);
    SDL_LockMutex(_outboundQueueLock);
    Info("Inbound packets: " << _inboundPackets);
    Info("Outbound packets: " << _outboundPackets);
    Info("Dropped packets: " << _droppedPackets);
    Info("Sent packets: " << _sentPackets);
    Info("Received packets: " << _receivedPackets);
    SDL_UnlockMutex(_outboundQueueLock);
    SDL_UnlockMutex(_inboundQueueLock);
}
