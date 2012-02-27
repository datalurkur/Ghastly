#include <Network/Socket.h>

bool Socket::InitializeSocketLayer() {
#if SYS_PLATFORM == PLATFORM_WIN32
    WSADATA wsaData;
    return (WSAStartup(MAKEWORD(2,2), &wsaData) == NO_ERROR);
#else
    return true;
#endif
}

void Socket::ShutdownSocketLayer() {
#if SYS_PLATFORM == PLATFORM_WIN32
    WSACleanup();
#endif
}

Socket::Socket(): _socketHandle(0) {}

Socket::~Socket() {}

bool Socket::isOpen() const {
    return _open;
}
