#ifndef GHASTLYPROTOCOL_H
#define GHASTLYPROTOCOL_H


struct GhastlyPacket {
    unsigned char type;
    unsigned short size;

    enum Types {
        FetchIDRequest,
        FetchIDResponse,
        DivulgeIDRequest,
        DivulgeIDResponse,
        Reserved
    };

    GhastlyPacket(unsigned char t, unsigned short s): type(t), size(s) {}
};

struct FetchIDRequest: public GhastlyPacket {
    FetchIDRequest():
        GhastlyPacket(GhastlyPacket::FetchIDRequest, sizeof(FetchIDRequest)) {}
};

struct FetchIDResponse: public GhastlyPacket {
    unsigned int id;

    FetchIDResponse(unsigned int assignedID):
        GhastlyPacket(GhastlyPacket::FetchIDResponse, sizeof(FetchIDResponse)), id(assignedID) {}
};

#endif
