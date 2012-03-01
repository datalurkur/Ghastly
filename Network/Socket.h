#ifndef SOCKET_H
#define SOCKET_H

#include <Base/Base.h>

#if SYS_PLATFORM == PLATFORM_WIN32
# include <winsock2.h>
# include <ws2tcpip.h>
# pragma comment(lib, "Ws2_32.lib")
#else
# include <sys/socket.h>
# include <netinet/in.h>
# include <fcntl.h>
# include <arpa/inet.h>
#endif

#include <SDL/SDL_mutex.h>

class Socket {
public:
    static bool InitializeSocketLayer();
    static void ShutdownSocketLayer();
	static bool IsSocketLayerReady();

private:
	static bool SocketLayerInitialized;

public:
    Socket();
    virtual ~Socket();

    bool isOpen() const;

    virtual bool openSocket() = 0;
    virtual void closeSocket() = 0;

    //SDL_mutex *getLock();

protected:
    SDL_mutex *_lock;
    bool _open;
    int _socketHandle;
};

#endif
