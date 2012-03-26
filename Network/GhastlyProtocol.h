#ifndef GHASTLYPROTOCOL_H
#define GHASTLYPROTOCOL_H

#include <Base/Base.h>
#include <Network/Packet.h>

/*
	This document defines how Ghastly clients and server communicate.  Ghastly attempts to be as stateless as possible, with minimal exception.
	Generally, a packet emitted by the client is denoted with '->', and a packet emitted by the server is denoted with '<-'.]
*/
namespace GhastlyProtocol {
	typedef uint8_t PayloadType;
	typedef uint16_t PayloadSize;
	
	struct Payload {
		PayloadType type;
		PayloadSize size;

		Payload(PayloadType t, PayloadSize s = sizeof(Payload)): type(t), size(s) {}
	};

	/*
	Host ID Assignment:
		Every host in the Ghastly Network model has a HostID which must be assigned it by the server.  This ID is acquired with the following exchange:
		-> Host ID Request (no payload)
		<- Host ID Assign  (Host ID)
	*/
	const PayloadType IDRequestType = 1;
	struct IDRequest: public Payload {
		IDRequest(): Payload(IDRequestType) {}
	};

	const PayloadType IDAssignType = 2;
	typedef uint16_t HostID;
	struct IDAssign: public Payload {
		HostID id;

		IDAssign(HostID i): Payload(IDAssignType, sizeof(IDAssign)), id(i) {}
	};

	/*
	Disconnection and Host ID reclamation:
		When a host wishes to terminate its connection for whatever reason, it issues a disconnect request.
		-> Disconnect (no payload)
		The server need not respond to this, and can now reclaim that host's ID
	*/
	const PayloadType DisconnectType = 3;
	struct Disconnect: public Payload {
		Disconnect(): Payload(DisconnectType) {}
	};

	/*
	Latency Discovery:
		In order to give clients a picture of overall server latency (above and beyond network latency), there is a ping tool available within the Ghastly Protocol which is relatively straightforward:
		-> Ping Request  (client timestamp)
		<- Ping Response (client timestamp, server timestamp)
		-> Ping Done     (server timestamp)
	*/

	struct GhastlyPacket: public Packet {
		inline GhastlyPacket(const NetAddress &addr, const Payload &payload): Packet(addr, (char*)&payload, payload.size) {}
	};
}

#endif