#ifndef PHYSICSENTITY_H
#define PHYSICSENTITY_H

#include <Engine/Entity.h>
#include <Engine/PhysicsController.h>

class PhysicsEngine;

class PhysicsEntity: public Entity {
public:
    static const std::string NodeType;

public:
    PhysicsEntity(const std::string &name);
    ~PhysicsEntity();

    virtual void setupPhysics(PhysicsEngine *physics) {}

protected:
    PhysicsEntity(const std::string &name, const std::string &type);

protected:    
    // Certain controllers we want direct access to, for things like updating the sizes of bodies
    PhysicsController *_physicsController;
};

#endif
