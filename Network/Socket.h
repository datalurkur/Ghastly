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
#endif

class Socket {
public:
    static bool InitializeSocketLayer();
    static void ShutdownSocketLayer();

public:
    Socket();
    virtual ~Socket();

    virtual bool open() = 0;
    virtual void close() = 0;

    bool isOpen() const;

protected:
    bool _open;
    int _socketHandle;
};

#endif
