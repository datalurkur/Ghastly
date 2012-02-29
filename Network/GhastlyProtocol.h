#ifndef GHASTLYPROTOCOL_H
#define GHASTLYPROTOCOL_H

#include <Network/NetAddress.h>

const unsigned char GHASTLY_PROTOCOL_VERSION = 1;

// Generic packet structure
struct GhastlyPacket {
    unsigned char type;
    unsigned short size;
	enum Types {
		NullPacket = 0,
		VersionPacket,
		IDNegotiationPackets // Informs IDNegotiation where its Type enum should begin
	};
    GhastlyPacket(unsigned char t, unsigned short s): type(t), size(s) {}
};

struct NullPacket: public GhastlyPacket {
	NullPacket(): GhastlyPacket(GhastlyPacket::NullPacket, sizeof(NullPacket)) {}
};

struct VersionPacket: public GhastlyPacket {
	unsigned char version;
	VersionPacket(): GhastlyPacket(GhastlyPacket::VersionPacket, sizeof(VersionPacket)), version(GHASTLY_PROTOCOL_VERSION) {}
};

// Any packet used in negotiating IDs between the server and peers
struct IDNegotiation: public GhastlyPacket {
    enum Types {
        AcquireID = GhastlyPacket::IDNegotiationPackets,
        AssignID,
		QueryID,
		DivulgeID,
		QueryPeerID,
		DivulgePeerID,
		IDNotFound,
        IDNegotiationTypesEnd
    };
	IDNegotiation(unsigned char t, unsigned short s): GhastlyPacket(t,s) {}
};

// Requests that the server assign an ID for this client
struct AcquireID: public IDNegotiation {
    AcquireID():
        IDNegotiation(IDNegotiation::AcquireID, sizeof(AcquireID)) {}
};

// The server responds with an assigned ID
struct AssignID: public IDNegotiation {
    unsigned int id;
    AssignID(unsigned int assignedID):
        IDNegotiation(IDNegotiation::AssignID, sizeof(AssignID)), id(assignedID) {}
};

// Queries a peer for its ID
struct QueryID: public IDNegotiation {
    QueryID():
        IDNegotiation(IDNegotiation::QueryID, sizeof(QueryID)) {}
};

// Divulges this peer's ID to a requester
struct DivulgeID: public IDNegotiation {
    unsigned int id;
    DivulgeID(unsigned int myID):
        IDNegotiation(IDNegotiation::DivulgeID, sizeof(DivulgeID)), id(myID) {}
};

// Queries a peer for the address of another peer based on its ID
struct QueryPeerID: public IDNegotiation {
	NetAddress addr;
    QueryPeerID(const NetAddress &unknownAddr):
        IDNegotiation(IDNegotiation::QueryPeerID, sizeof(QueryPeerID)), addr(unknownAddr) {}
};

// Divulges the requested peer's address (returning its ID to keep the protocol mostly stateless)
// Not the preferred way of doing peer discovery, since we have to send a NetAddress structure over the wire
struct DivulgePeerID: public IDNegotiation {
	NetAddress addr;
    unsigned int id;
    DivulgePeerID(const NetAddress &unknownAddr, unsigned int unknownID):
        IDNegotiation(IDNegotiation::DivulgePeerID, sizeof(DivulgePeerID)), addr(unknownAddr), id(unknownID) {}
};

#endif
