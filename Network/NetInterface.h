#ifndef NETINTERFACE_H
#define NETINTERFACE_H

#include <Network/NetAddress.h>
#include <Network/GhastlyProtocol.h>
#include <Network/PacketListener.h>

const unsigned int DEFAULT_MAX_PACKET_SIZE = 1024;

class NetInterface {
public:
    NetInterface();
	NetInterface(const NetAddress &server);
    virtual ~NetInterface();

    void setMaxPacketSize(unsigned int maxSize);
    unsigned int getMaxPacketSize() const;

	virtual void processPackets() = 0;
	bool send(NetworkID id, const GhastlyPacket &packet);

	void registerPacketListener(PacketListener *listener);
	void unregisterPacketListener(PacketListener *listener);

protected:
	virtual bool send(const NetAddress &addr, const GhastlyPacket &packet) = 0;

	void setup();
	void teardown();

    unsigned int getNetworkID(const NetAddress &addr) const;
    const NetAddress& getNetAddress(NetworkID id) const;

    bool netAddressKnown(NetworkID id) const;
    bool setNetworkID(const NetAddress &addr, NetworkID id);

	void processPacket(NetworkID id, const NetAddress &addr, const GhastlyPacket *packet, bool mapped);

protected:
    bool _server;
    unsigned int _clientID;

    unsigned int _maxPacketSize;

private:
    typedef std::map<NetworkID,NetAddress> NetIDMap;
    NetIDMap _idMap;

	typedef std::list<PacketListener*> ListenerList;
	ListenerList _listeners;
};

#endif
