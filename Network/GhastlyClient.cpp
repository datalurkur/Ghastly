#include <Network/GhastlyClient.h>
#include <Base/Assertion.h>

GhastlyClient::GhastlyClient(): GhastlyHost(ID_UNASSIGNED), _state(NOT_CONNECTED) {
}

GhastlyClient::~GhastlyClient() {
    disconnect();
}

ClientState GhastlyClient::getState() const {
    return _state;
}

void GhastlyClient::update(int elapsed) {
    Packet packet;
    while(recvPacket(packet)) {
        onPacketReceive(packet);
    }

    switch(_state) {
    //case NOT_CONNECTED:
    //    break;
    case AWAITING_ID: {
        IDRequest idreq;
        sendPacket(Packet(_server, (char*)&idreq, sizeof(idreq)));
        break;
    }
    case AWAITING_DATA:
        _state = READY;
        break;
    //case READY:
    //    break;
    //default:
    //    Error("Unknown client state " << _state);
    //    break;
    };
}

void GhastlyClient::onPacketReceive(const Packet &packet) {
    Payload *payload = (Payload*)packet.data;
    switch(payload->type) {
    case IDAssignType:
        if(_state == AWAITING_ID) {
            _id = ((IDAssign*)payload)->id;
            //_state = AWAITING_DATA;
            _state = READY;
            Info("Got ID " << _id << " from server");
        } else {
            Warn("Already had ID " << _id << " but got id " << ((IDAssign*)payload)->type << " from server");
        }
        break;
    case HostRejectType:
        if(_state == AWAITING_ID) {
            _state = NOT_CONNECTED;
            Info("Server is full");
        } else {
            Warn("Received an unexpected HostReject message");
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
        IDRequest idReq;
        sendPacket(Packet(_server, (char*)&idReq, sizeof(idReq)));
    }
}

void GhastlyClient::disconnect() {
    if(_state != NOT_CONNECTED) {
        Disconnect dc;
        sendPacket(Packet(_server, (char*)&dc, sizeof(dc)));
        _state = NOT_CONNECTED;
    }
}
