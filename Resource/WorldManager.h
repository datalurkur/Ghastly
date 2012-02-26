#ifndef WORLDMANAGER_H
#define WORLDMANAGER_H

#include <Engine/World.h>
#include <Resource/ThreadedResourceManager.h>

class WorldManager;

class WorldManager: public ThreadedResourceManager<World, WorldManager> {
protected:
    static void DoLoad(const std::string &name, World *world);
    static const std::string LoadDirectory;

    friend class ThreadedResourceManager<World, WorldManager>;
    friend class ResourceManager<World, WorldManager>;
};

#endif
