#include <Network/ListenSocket.h>
#include <Base/Assertion.h>
#include <Base/Log.h>

int InvokeListenSocketLoop(void *params) {
    ListenSocket *socket = (ListenSocket*)params;
    socket->doListening();
    return 1;
}

ListenSocket::ListenSocket(SocketCreationListener *acceptListener): Socket(true), _acceptListener(acceptListener), _listenThread(0), _listenMutex(0), _shouldDie(false) {
}

ListenSocket::~ListenSocket() {
    stopListening();
}

bool ListenSocket::startListening(unsigned short localPort) {
    if(!_listenThread) {
        if(!createSocket(SOCK_STREAM, IPPROTO_TCP)) { return false; }
        if(!bindSocket(localPort)) { return false; }

        // Listen for connections, setting the backlog to 5
        SDL_LockMutex(_lock);
        listen(_socketHandle, 5);
        SDL_UnlockMutex(_lock);

        // Start looping to accept connections
        _listenMutex = SDL_CreateMutex();
        _listenThread = SDL_CreateThread(InvokeListenSocketLoop, "ListenSocketThread", (void*)this);

        Info("Listening for connections on port " << localPort);

        return true;
    } else { return false; }
}

void ListenSocket::stopListening() {
    if(_listenThread) {
        int status;
        
        // Tell the thread to die
        SDL_mutexP(_listenMutex);
        _shouldDie = true;
        SDL_mutexV(_listenMutex);
        
        // Wait for the thread to die
        SDL_WaitThread(_listenThread, &status);
        
        // Teardown
        SDL_DestroyMutex(_listenMutex);
        _listenThread = 0;
        closeSocket();

        Info("ListenSocked closed");
    }
}

void ListenSocket::doListening() {
    while(true) {
        sockaddr_in clientAddr;
        socklen_t clientAddrLength;
        int newSocketHandle;
        
        SDL_mutexP(_listenMutex);
        if(_shouldDie) {
            SDL_mutexV(_listenMutex);
            break;
        } else {
            SDL_mutexV(_listenMutex);
        }

        clientAddrLength = sizeof(clientAddr);
        newSocketHandle = accept(_socketHandle, (sockaddr*)&clientAddr, &clientAddrLength);

        if(newSocketHandle <= 0) {
            // Fail gracefully and go on to processing the next connection
            //Error("ListenSocket failed to accept incoming connection");
            continue;
        }

        NetAddress clientAddress(&clientAddr);
        Info("Incoming connection from " << clientAddress);

        TCPSocket *newSocket = new TCPSocket(newSocketHandle, false);
        newSocket->setBlockingFlag(false);
        if(!_acceptListener->onSocketCreation(clientAddress, newSocket)) {
            delete newSocket;
        }
    }
}
