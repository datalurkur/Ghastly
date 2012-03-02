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
    Socket(bool blocking);
    virtual ~Socket();

    bool createSocket(int type, int proto = 0);
    bool bindSocket(unsigned short localPort);
    void closeSocket();

    bool isOpen() const;

protected:
    typedef unsigned char SocketState;
    enum SocketStates {
        Uninitialized = 0,
        Created,
        Bound,
        Listening,
        Connected,
        Closed
    };

protected:
    SDL_mutex *_lock;

    SocketState _state;
    bool _blocking;
    int _socketHandle;
};

#endif
