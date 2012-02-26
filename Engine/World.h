#ifndef WORLD_H
#define WORLD_H

#include <Base/Base.h>
#include <Base/Matrix4.h>
#include <Engine/Entity.h>
#include <Engine/QuadTreeSceneManager.h>
#include <Engine/PhysicsEngine.h>

class Camera;
class RenderContext;
class WorldManager;

class World {
public:
	World();
	virtual ~World();

	virtual void load(const std::string &name) {}

    // Update the world objects
    void update(int elapsed);

	// Render the world
	void render(Camera *camera, RenderContext *context);

	// Return the scene manager
	SceneManager *getScene();
    
    // Designed to generically add objects to the scene
    template <typename T>
    T *createObject(const std::string &name);

    // Designed to generically add an entity to the entities list
    // Defers to createObject to also add the entity to the scene
    template <typename T>
    T *createEntity(const std::string &name);
    
    template <typename T>
    void addEntity(T* t);

protected:
	QuadTreeSceneManager *_scene;

    typedef std::map<std::string, Entity*> EntityList;
    EntityList _entities;
    
    // Physics world object
    PhysicsEngine *_physics;

    // Objects created by the world factory
    friend class WorldManager;
};

template <typename T>
T* World::createObject(const std::string &name) {
    T *t = new T(name);
    _scene->addNode(t);
    return t;
}

template <typename T>
T* World::createEntity(const std::string &name) {
    T *t;
    t = new T(name);
	addEntity(t);
    return t;
}

template <typename T>
void World::addEntity(T* t) {
    _scene->addNode(t);
    _entities[t->getName()] = t;
    t->setupPhysics(_physics);
}

#endif
