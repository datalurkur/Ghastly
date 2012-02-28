#ifndef UDPINTERFACE_H
#define UDPINTERFACE_H

#include <Network/UDPSocket.h>

class UDPInterface: public NetInterface {
public:
    UDPInterface(UDPSocket *socket);
    virtual ~UDPInterface();

private:
    UDPSocket *_socket;
};

#endif
