#include <Engine/World.h>

#include <Base/Log.h>
#include <Render/RenderContext.h>

World::World() {
	_scene = new QuadTreeSceneManager();
    _physics = new PhysicsEngine();
}

World::~World() {
	delete _scene;
	delete _physics;
}

void World::update(int elapsed) {
    EntityList::iterator itr;
    
	// Tick the physics simulation
    _physics->update(elapsed);

    // Update the entities (and their controllers)
    for(itr = _entities.begin(); itr != _entities.end(); itr++) {
        itr->second->update(elapsed);
    }

    // Update the scene
    _scene->update();
}

void World::render(Camera *camera, RenderContext *context) {
	_scene->render(camera, context);
}

SceneManager* World::getScene() {
	return _scene;
}

