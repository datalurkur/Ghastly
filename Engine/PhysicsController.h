#ifndef PHYSICSCONTROLLER_H
#define PHYSICSCONTROLLER_H

#include <Box2D/Box2D.h>
#include <Engine/Controller.h>
#include <Engine/PhysicsEngine.h>

class PhysicsController: public Controller {
public:
    PhysicsController(PhysicsEngine *engine, SceneNode *node);
    virtual ~PhysicsController();

    void update(int elapsed);
    
    void updatePosition(const Vector2 &pos);
    
    void setBody(b2Body *body);
	b2Body *getBody();
    
    void disallowUpdates();
    void allowUpdates();
    
    PhysicsEngine *getEngine();

private:
    PhysicsEngine *_engine;
    b2Body *_body;

    // Used to turn off updating for static bodies
    bool _updates;
};

#endif
