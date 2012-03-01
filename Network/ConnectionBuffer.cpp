#include <Network/ConnectionBuffer.h>

int InvokeConnectionBufferThreadFunction(void *params) {
    ConnectionBuffer *cBuffer = (ConnectionBuffer*)params;
    cBuffer->doBuffering();
    return 1;
}

unsigned int ConnectionBuffer::DefaultMaxBufferSize = 5096;

unsigned int ConnectionBuffer::DefaultMaxPacketSize = 1024;

ConnectionBuffer::ConnectionBuffer(): _packetBuffer(0), _maxBufferSize(DefaultMaxBufferSize), _maxPacketSize(DefaultMaxPacketSize),
    _droppedPackets(0), _receivedPackets(0), _sentPackets(0), _inboundPackets(0), _outboundPackets(0) {
    _lock = SDL_CreateMutex();
    _bufferLock = SDL_CreateMutex();

    startBuffering();
}

ConnectionBuffer::~ConnectionBuffer() {
    stopBuffering();
    if(_lock) {
        SDL_DestroyMutex(_lock);
        _lock = 0;
    }
    if(_bufferLock) {
        SDL_DestroyMutex(_bufferLock);
        _bufferLock = 0;
    }
}

void ConnectionBuffer::startBuffering() {
    if(!_bufferThread) {
        _packetBuffer = (char*)calloc(_maxPacketSize, sizeof(char));
        _bufferThread = SDL_CreateThread(InvokeConnectionBufferThreadFunction, (void*)this);
    }
}

void ConnectionBuffer::stopBuffering() {
    if(_bufferThread) {
        SDL_mutexP(_lock);
        SDL_KillThread(_bufferThread);
        SDL_mutexV(_lock);
        _bufferThread = 0;

        free(_packetBuffer);
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
    _outbound.push(packet);
    if(_outbound.size() > _maxBufferSize) {
        _outbound.pop();
        _droppedPackets++;
        return false;
    } else {
        _outboundPackets++;
        return true;
    }
}

bool ConnectionBuffer::consumePacket(Packet &packet) {
    if(_inbound.empty()) { return false; }
    else {
        packet = _inbound.front();
        _inbound.pop();
        _inboundPackets--;
        return true;
    }
}
