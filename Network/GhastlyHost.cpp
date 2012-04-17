#include <Network/GhastlyHost.h>
#include <Base/Log.h>

GhastlyHost::GhastlyHost(HostID id): _id(id) {
}

void GhastlyHost::handleCustomPayload(Payload *payload) {
    Warn("Custom payload handler not defined; unknown payload type " << payload->type << " will be ignored.");
}