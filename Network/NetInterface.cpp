#include <Network/NetInterface.h>

NetInterface::NetInterface() {
}

NetInterface::~NetInterface() {
}

unsigned int NetInterface::getNetworkID(const NetAddress &addr) const {
    NetIDMap::iterator itr;
    for(itr = _idMap.begin(); itr != _idMap.end(); itr++) {
        if(itr->second == addr) {
            return itr->first;
        }
    }
    return ID_NOT_FOUND;
}

const NetAddress& getNetworkAddress(unsigned int id) const {
    return _idMap[id];
}
