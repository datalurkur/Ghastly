#ifndef NETINTERFACE_H
#define NETINTERFACE_H

#include <Network/NetAddress.h>

typedef unsigned int NetworkID;
const NetworkID ID_NOT_FOUND = 0xffffffff;

class NetInterface {
public:
    NetInterface();
    virtual ~NetInterface();

protected:
    unsigned int getNetworkID(const NetAddress &addr) const;
    const NetAddress& getNetworkAddress(unsigned int id) const;

    bool assignNetworkID(const NetAddress &addr);

private:
    typedef std::map<NetworkID,NetAddress> NetIDMap;
    NetIDMap _idMap;
};

#endif
