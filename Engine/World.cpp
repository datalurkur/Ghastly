#include <Engine/World.h>

#include <Base/Log.h>
#include <Render/RenderContext.h>

World::World(bool usesPhysics = false) _physics(0), _usesPhysics(usesPhysics) {
    _scene = new QuadTreeSceneManager();
    if(_usesPhysics) {
        _physics = new PhysicsEngine();
    }
}

World::~World() {
    delete _scene;
    if(_physics) {
        delete _physics;
    }
}

void World::update(int elapsed) {
    EntityList::iterator itr;
    
    if(_usesPhysics) {
        // Tick the physics simulation
        _physics->update(elapsed);
    }

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

