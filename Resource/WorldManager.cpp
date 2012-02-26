#include <Resource/WorldManager.h>
#include <Base/Vector2.h>

const std::string WorldManager::LoadDirectory = "Save";

void WorldManager::DoLoad(const std::string &name, World *world) {
	// This seems like a really roundabout way of loading a world
	// Then again, it's nice to have a unified way to load resources that the World resource is a part of,
	//  and still retain the ability to subclass world and have it do special load stuff
	world->load(name);
}
