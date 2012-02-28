#ifndef UDPINTERFACE_H
#define UDPINTERFACE_H

#include <Network/UDPSocket.h>

class UDPInterface: public NetInterface {
public:
    UDPInterface(UDPSocket *socket);
    virtual ~UDPInterface();

    void recv(unsigned int &id, char *data, unsigned int &size);
    bool send(unsigned int id, const char *data, unsigned int size);

private:
    UDPSocket *_socket;
};

#endif
