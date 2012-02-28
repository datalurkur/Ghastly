#ifndef NETINTERFACE_H
#define NETINTERFACE_H

#include <Network/NetAddress.h>

typedef unsigned int NetworkID;
const NetworkID ID_NOT_FOUND = 0xffffffff;

const unsigned int DEFAULT_MAX_PACKET_SIZE = 1024;

class NetInterface {
public:
    NetInterface();
    virtual ~NetInterface();

    virtual void recv(unsigned int &id, char *data, unsigned int &size) = 0;
    virtual bool send(unsigned int id, const char *data, unsigned int size) = 0;

    // Responsible for negotiating packets from addresses that don't yet have an ID
    void processUnmappedPacket(const Address &addr, const char *data, unsigned int size);

    void setMaxPacketSize(unsigned int maxSize);
    unsigned int getMaxPacketSize() const;

protected:
    unsigned int getNetworkID(const NetAddress &addr) const;
    const NetAddress& getNetworkAddress(unsigned int id) const;

    bool setNetworkID(const NetAddress &addr, unsigned int id);

private:
    typedef std::map<NetworkID,NetAddress> NetIDMap;
    NetIDMap _idMap;

    unsigned int _maxPacketSize;

    unsigned int _clientID;
    bool _server;
};

#endif
