#ifndef UDPINTERFACE_H
#define UDPINTERFACE_H

#include <Network/UDPSocket.h>
#include <Network/NetInterface.h>

class UDPInterface: public NetInterface {
public:
    UDPInterface(unsigned short port);
	UDPInterface(unsigned short port, const NetAddress &server);
    virtual ~UDPInterface();

	void processPackets();

protected:
	bool send(const NetAddress &addr, const GhastlyPacket &packet);

private:
	void setup(unsigned short port);
	void teardown();

private:
    UDPSocket *_socket;
};

#endif
