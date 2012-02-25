#ifndef ENTITY_H
#define ENTITY_H

#include <Engine/SceneNode.h>
#include <Engine/Controller.h>
#include <Engine/PhysicsController.h>

class PhysicsEngine;

class Entity: public SceneNode {
public:
	static const std::string NodeType;

public:
    Entity(const std::string &name);
    ~Entity();

    void update(int elapsed);
    
    template <typename C, typename T>
    C* addController(T* controlObject);

    virtual void setupPhysics(PhysicsEngine *physics) {}

protected:
    Entity(const std::string &name, const std::string &type);

private:
    ControllerList _controllers;

protected:    
    // Certain controllers we want direct access to, for things like updating the sizes of bodies
    PhysicsController *_physicsController;
};

#endif
