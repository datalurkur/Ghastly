#include <Network/NetInterface.h>
#include <Base/Log.h>

NetInterface::NetInterface(): _server(true), _clientID(0), _maxPacketSize(DEFAULT_MAX_PACKET_SIZE) {
}

NetInterface::~NetInterface() {
}

void NetInterface::processUnmappedPacket(const NetAddress &addr, const char *data, unsigned int size) {

}

void NetInterface::setMaxPacketSize(unsigned int maxSize) {
    _maxPacketSize = maxSize;
}

unsigned int NetInterface::getMaxPacketSize() const {
    return _maxPacketSize;
}

unsigned int NetInterface::getNetworkID(const NetAddress &addr) const {
    NetIDMap::const_iterator itr;
    for(itr = _idMap.begin(); itr != _idMap.end(); itr++) {
        if(itr->second == addr) {
            return itr->first;
        }
    }
    return ID_NOT_FOUND;
}

const NetAddress& NetInterface::getNetworkAddress(NetworkID id) const {
    NetIDMap::const_iterator itr = _idMap.find(id);
    ASSERT(itr != _idMap.end());
    return itr->second;
}

bool NetInterface::netIDKnown(NetworkID id) const {
    return (_idMap.find(id) != _idMap.end());
}

bool NetInterface::setNetworkID(const NetAddress &addr, NetworkID id) {
    if(_idMap.find(id) == _idMap.end()) {
        _idMap[id] = addr;
        return true;
    } else {
        if(_idMap[id] != addr) {
            Warn("Network ID " << id << " is bound to another address.");
        }
        Info("Network ID " << id << " is already set.");
        return false;
    }
}
