#include <Network/Socket.h>
#include <Base/Log.h>

bool Socket::InitializeSocketLayer() {
#if SYS_PLATFORM == PLATFORM_WIN32
	WSADATA wsaData;

    if(SocketLayerInitialized) { return true; }
	else if(WSAStartup(MAKEWORD(2,2), &wsaData) == NO_ERROR) {
		SocketLayerInitialized = true;
	}
#endif
	return IsSocketLayerReady();
}

void Socket::ShutdownSocketLayer() {
#if SYS_PLATFORM == PLATFORM_WIN32
    WSACleanup();
	SocketLayerInitialized = false;
#endif
}

bool Socket::IsSocketLayerReady() {
#if SYS_PLATFORM == PLATFORM_WIN32
	return SocketLayerInitialized;
#else
	return true;
#endif
}

bool Socket::SocketLayerInitialized = false;

Socket::Socket(): _socketHandle(0), _open(false) {
	if(!IsSocketLayerReady()) {
		Warn("Socket layer not yet initialized! Please call InitializeSocketLayer if you expect your sockets to send data.");
	}
    _lock = SDL_CreateMutex();
}

Socket::~Socket() {
    if(_lock) {
        SDL_DestroyMutex(_lock);
        _lock = 0;
    }
}

bool Socket::isOpen() const {
    return _open;
}

//SDL_mutex *Socket::getLock() { return _lock; }
