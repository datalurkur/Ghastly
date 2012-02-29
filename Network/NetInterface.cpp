#include <Network/NetInterface.h>
#include <Base/Log.h>

NetInterface::NetInterface(): _server(true), _clientID(SERVER_ID), _maxPacketSize(DEFAULT_MAX_PACKET_SIZE) {
}

NetInterface::NetInterface(const NetAddress &server): _server(false), _clientID(SERVER_ID), _maxPacketSize(DEFAULT_MAX_PACKET_SIZE) {
	_idMap[SERVER_ID] = server;
}

NetInterface::~NetInterface() {
}

void NetInterface::setup() {
	if(!_server) {
		send(SERVER_ID, AcquireID());
	}
}

void NetInterface::teardown() {
}

void NetInterface::registerPacketListener(PacketListener *listener) {
	ListenerList::iterator itr;
	for(itr = _listeners.begin(); itr != _listeners.end(); itr++) {
		if(*itr == listener) { return; }
	}
	_listeners.push_back(listener);
	listener->setInterface(this);
}

void NetInterface::unregisterPacketListener(PacketListener *listener) {
	ListenerList::iterator itr;
	for(itr = _listeners.begin(); itr != _listeners.end(); itr++) {
		if(*itr == listener) { break; }
	}
	if(itr != _listeners.end()) {
		_listeners.erase(itr);
	}
}

bool NetInterface::send(NetworkID id, const GhastlyPacket &packet) {
	NetAddress rcpt;

    // Break up the packet into multiples based on max payload size
    ASSERT(packet.size <= _maxPacketSize); // Has yet to be implemented

    // Obviously we can't send data to an address we don't know
    ASSERT(netAddressKnown(id));
    rcpt = getNetAddress(id);

	return send(rcpt, packet);
}

void NetInterface::processPacket(NetworkID id, const NetAddress &addr, const GhastlyPacket *packet, bool mapped) {
	switch(packet->type) {
	case IDNegotiation::AcquireID: {
		if(_server) {
			// Respond to the requester with the next available ID
			NetworkID nextID;
			NetIDMap::const_iterator itr;

			nextID = 1;
			for(itr = _idMap.begin(); itr != _idMap.end(); itr++) {
				if(itr->first >= nextID) { nextID = itr->first + 1; }
			}

			_idMap[nextID] = addr;
			Debug("Assigning ID " << nextID << " to client.");
			send(addr, AssignID(nextID));
		} else {
			// Inform the requester that we are indeed not the server, we are _clientID
			Debug("Silly peer, we're not the server, we're " << _clientID << "!");
			send(addr, DivulgeID(_clientID));
		}
		break;
	}
	case IDNegotiation::AssignID: {
		if(!_server && _clientID == SERVER_ID) {
			AssignID *assignPacket = (AssignID*)packet;

			_clientID = assignPacket->id;

			Debug("Received ID " << _clientID << " from the server.");
		} else {
			// Someone is trying to assign us an ID?  The hell?
			Warn("Someone is trying to assign us an ID, either redundantly or by mistake.");
		}
		break;
	}
	case IDNegotiation::QueryID: {
		if(_server || _clientID != SERVER_ID) {
			Debug("Divulging ID " << _clientID);
			send(addr, DivulgeID(_clientID));
		} else if(!_server) {
			// We don't have an ID yet!  Get one!
			Debug("No ID acquired yet, attempting to acquire one from the server.");
			send(SERVER_ID, AcquireID());
		} else {
			// I honestly don't know how we wound up here
			Error("Somehow we are the server and don't have our own ID. Be very afraid.");
			ASSERT(0);
		}
		break;
	}
	case IDNegotiation::DivulgeID: {
		DivulgeID *divulgePacket = (DivulgeID*)packet;

		NetworkID foreignID = divulgePacket->id;

		if(netAddressKnown(foreignID)) {
			// We already know this ID, make sure the addresses match up
			if(getNetAddress(foreignID) != addr) {
				Error("ID " << foreignID << " is already registered to another address!");
			}
		} else {
			Debug("Associating ID " << foreignID << " with peer.");
			_idMap[foreignID] = addr;
		}
		break;
	}
	case IDNegotiation::QueryPeerID: {
		QueryPeerID *queryPacket = (QueryPeerID*)packet;

		NetAddress foreignAddr;
		NetworkID foreignID = getNetworkID(foreignAddr);

		if(foreignID == ID_NOT_FOUND) {
			// We know this peer's ID, pass it on to the requester
			Debug("Divulging peer ID " << foreignID << " to requester.");
			send(addr, DivulgePeerID(foreignAddr, foreignID));
		} else {
			// We've never heard of this peer
			Debug("Information requested for unknown peer.");
		}
		break;
	}
	case IDNegotiation::DivulgePeerID: {
		DivulgePeerID *divulgePeerPacket = (DivulgePeerID*)packet;

		NetworkID foreignID    = divulgePeerPacket->id;
		NetAddress foreignAddr = divulgePeerPacket->addr;

		if(_idMap.find(foreignID) != _idMap.end()) {
			// We already know of this peer
			if(getNetAddress(foreignID) != foreignAddr) {
				Error("ID " << foreignID << " is already registered to another address!");
			}
		} else {
			Debug("Associating ID " << foreignID << " with peer.");
			_idMap[foreignID] = foreignAddr;
		}
		break;
	}
	default: {
		Debug("Uncertain what to do with packet of type " << packet->type);
		break;
	}
	}
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

const NetAddress& NetInterface::getNetAddress(NetworkID id) const {
    NetIDMap::const_iterator itr = _idMap.find(id);
    ASSERT(itr != _idMap.end());
    return itr->second;
}

bool NetInterface::netAddressKnown(NetworkID id) const {
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
