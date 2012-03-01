#include <Network/ConnectionBuffer.h>
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
    _inboundThread(0), _outboundThread(0),
    _packetBuffer(0), _maxBufferSize(DefaultMaxBufferSize), _maxPacketSize(DefaultMaxPacketSize),
    _droppedPackets(0), _receivedPackets(0), _sentPackets(0), _inboundPackets(0), _outboundPackets(0)
{
    _inboundQueueLock  = SDL_CreateMutex();
    _outboundQueueLock = SDL_CreateMutex();
    _bufferLock        = SDL_CreateMutex();
}

ConnectionBuffer::~ConnectionBuffer() {
    stopBuffering();
    SDL_DestroyMutex(_inboundQueueLock);
    SDL_DestroyMutex(_outboundQueueLock);
    SDL_DestroyMutex(_bufferLock);
}

void ConnectionBuffer::startBuffering() {
    if(!_inboundThread) {
        Debug("Inbound packet buffering begins");
        _packetBuffer = (char*)calloc(_maxPacketSize, sizeof(char));
        //_inboundThread = SDL_CreateThread(InvokeInboundConnectionBufferThreadFunction, (void*)this);
    }
    if(!_outboundThread) {
        Debug("Outbound packet buffering begins");
        _outboundThread = SDL_CreateThread(InvokeOutboundConnectionBufferThreadFunction, (void*)this);
    }
}

void ConnectionBuffer::stopBuffering() {
    if(_inboundThread) {
        Debug("Inbound packet buffering ends");
        SDL_mutexP(_inboundQueueLock);
        SDL_KillThread(_inboundThread);
        SDL_mutexV(_inboundQueueLock);
        _inboundThread = 0;

        free(_packetBuffer);
        _packetBuffer = 0;
    }
    if(_outboundThread) {
        Debug("Outbound packet buffering ends");
        SDL_mutexP(_outboundQueueLock);
        SDL_KillThread(_outboundThread);
        SDL_mutexV(_outboundQueueLock);
        _outboundThread = 0;
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
    SDL_mutexP(_bufferLock);
    _packetBuffer = (char*)realloc(_packetBuffer, _maxPacketSize*sizeof(char));
    SDL_mutexV(_bufferLock);
}

unsigned int ConnectionBuffer::getMaxPacketSize() {
    return _maxPacketSize;
}

bool ConnectionBuffer::providePacket(const Packet &packet) {
    bool ret;

    SDL_mutexP(_outboundQueueLock);
    _outbound.push(packet);
    if(_outbound.size() > _maxBufferSize) {
        _outbound.pop();
        SDL_mutexP(_inboundQueueLock);
        _droppedPackets++;
        SDL_mutexV(_inboundQueueLock);
        ret = false;
    } else {
        _outboundPackets++;
        ret = true;
    }
    SDL_mutexV(_outboundQueueLock);

    return ret;
}

bool ConnectionBuffer::consumePacket(Packet &packet) {
    bool ret;

    SDL_mutexP(_inboundQueueLock);
    if(_inbound.empty()) { ret = false; }
    else {
        packet = _inbound.front();
        _inbound.pop();
        _inboundPackets--;
        ret = true;
    }
    SDL_mutexV(_inboundQueueLock);

    return ret;
}
