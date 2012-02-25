#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <Base/Vector2.h>
#include <Box2D/Box2D.h>
#include <Engine/ContactListener.h>

// Some entites will need more granular control over their body and 
//  fixtures than the functions of this class will provide

class PhysicsEngine: public b2ContactListener {
public:
    PhysicsEngine();
    virtual ~PhysicsEngine();

    void update(int elapsed);

    // Generic do-it-yourself functions
    b2World *getPhysicsWorld();
    void destroyObject(b2Body *body);
    
    // High-level builder functions
    b2Body *createStaticBox(const Vector2 &pos, const Vector2 &dim);
    b2Body *createStaticChain(const std::vector<Vector2> &verts, bool loop=false);
    b2Body *createDynamicBox(const Vector2 &pos, const Vector2 &dim, float density, float friction, bool canRotate = true);

    // Contact callbacks
    // It's irritating that Box2D capitalizes all of its function names
    void BeginContact(b2Contact *contact);
    void EndContact(b2Contact *contact);

    void addFixtureContactListener(FixtureID *id, ContactListener *controller);
	void removeFixtureContactListener(FixtureID *id);

private:
    b2Vec2 _gravity;
    b2World *_world;

    int _stepSize, _velocityIterations, _positionIterations;

	typedef std::map<FixtureID*,ContactListener*> FixtureContactMap;
	FixtureContactMap _fixtureContactListeners;
};

#endif
