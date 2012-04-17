#ifndef GHASTLYHOST_H
#define GHASTLYHOST_H

#include <Network/GhastlyProtocol.h>
#include <Network/ConnectionProvider.h>

using namespace GhastlyProtocol;

class GhastlyHost {
public:
    enum {
        ID_UNASSIGNED = 0,
        ID_SERVER,
        ID_CLIENT
    };

public:
    GhastlyHost(HostID id = ID_UNASSIGNED);

    virtual void update(int elapsed) = 0;

protected:
    void handleCustomPayload(Payload *payload);

protected:
    HostID _id;
};

#endif
