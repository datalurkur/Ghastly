#ifndef ENTITY_H
#define ENTITY_H

#include <Engine/SceneNode.h>
#include <Engine/Controller.h>

class PhysicsEngine;

class Entity: public SceneNode<float> {
public:
    static const std::string NodeType;

public:
    Entity(const std::string &name);
    ~Entity();

    virtual void update(int elapsed);
    
    template <typename C, typename T>
    C* addController(T* controlObject);

protected:
    Entity(const std::string &name, const std::string &type);

private:
    ControllerList _controllers;
};

#endif
