#include <Network/GhastlyClient.h>
#include <Base/Assertion.h>

GhastlyClient::GhastlyClient(): GhastlyHost(ID_UNASSIGNED), _state(NOT_CONNECTED) {}

GhastlyClient::~GhastlyClient() {
	disconnect();
}

ClientState GhastlyClient::getState() const {
	return _state;
}

void GhastlyClient::update(int elapsed) {
	dispatchIncomingPackets();

	switch(_state) {
	//case NOT_CONNECTED:
	//	break;
	case AWAITING_ID:
		sendPacket(GhastlyPacket(_server, IDRequest()));
		break;
	case AWAITING_DATA:
		break;
	//case READY:
	//	break;
	//default:
	//	Error("Unknown client state " << _state);
	//	break;
	};
}

void GhastlyClient::onPacketReceive(const Packet &packet) {
	Payload *payload = (Payload*)packet.data;
	switch(payload->type) {
	case IDAssignType:
		if(_state == AWAITING_ID) {
			_id = ((IDAssign*)payload)->type;
			_state = AWAITING_DATA;
			Info("Got ID " << _id << " from server");
		} else {
			Warn("Already had ID " << _id << " but got id " << ((IDAssign*)payload)->type << " from server");
		}
		break;
	case DisconnectType:
		if(_state != NOT_CONNECTED) {
			_id = ID_UNASSIGNED;
			_state = NOT_CONNECTED;
			Info("Server disconnected");
		}
		break;
	default:
		handleCustomPayload(payload);
		break;
	}
}

void GhastlyClient::connect(const NetAddress &addr) {
	if(_state == NOT_CONNECTED) {
		_server = addr;
		_state  = AWAITING_ID;
		sendPacket(GhastlyPacket(_server, IDRequest()));
	}
}

void GhastlyClient::disconnect() {
	if(_state != NOT_CONNECTED) {
		sendPacket(GhastlyPacket(_server, Disconnect()));
		_state = NOT_CONNECTED;
	}
}