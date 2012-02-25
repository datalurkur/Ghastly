#include <Engine/PhysicsEngine.h>

const char EdgeID = 3;

PhysicsEngine::PhysicsEngine():
    _gravity(0.0, -10.0), _world(0),
    _stepSize(16), _velocityIterations(6), _positionIterations(2) // Roughly 1/60th of a second step
{
    _world = new b2World(b2Vec2(_gravity));
    _world->SetContactListener(this);
    _world->ClearForces();
}

PhysicsEngine::~PhysicsEngine() { 
    delete _world;
    _world = 0;
}

void PhysicsEngine::update(int elapsed) {
    static int leftOver = 0;
    leftOver += elapsed;
    while(leftOver > _stepSize) {
        leftOver -= _stepSize;
        _world->Step(_stepSize / 1000.0f, _velocityIterations, _positionIterations);
    }
    _world->ClearForces();
}

b2World *PhysicsEngine::getPhysicsWorld() {
    return _world;
}

void PhysicsEngine::destroyObject(b2Body *body) {
    _world->DestroyBody(body);
}

void PhysicsEngine::BeginContact(b2Contact *contact) {
    FixtureContactMap::iterator itr = _fixtureContactListeners.begin();
    for(; itr != _fixtureContactListeners.end(); itr++) {
        FixtureID *a, *b;
        a = (FixtureID*)contact->GetFixtureA()->GetUserData();
        b = (FixtureID*)contact->GetFixtureB()->GetUserData();
        if(itr->first == a) {
            itr->second->contactBegins(a, b);
        } else if(itr->first == b) {
			itr->second->contactBegins(b, a);
		}
    }   
}

void PhysicsEngine::EndContact(b2Contact *contact) {
    FixtureContactMap::iterator itr = _fixtureContactListeners.begin();
    for(; itr != _fixtureContactListeners.end(); itr++) {
        FixtureID *a, *b;
        a = (FixtureID*)contact->GetFixtureA()->GetUserData();
        b = (FixtureID*)contact->GetFixtureB()->GetUserData();
        if(itr->first == a) {
            itr->second->contactEnds(a, b);
        } else if(itr->first == b) {
			itr->second->contactEnds(b, a);
		}
    }
}

void PhysicsEngine::addFixtureContactListener(FixtureID *id, ContactListener *controller) {
    _fixtureContactListeners[id] = controller;
}

void PhysicsEngine::removeFixtureContactListener(FixtureID *id) {
	FixtureContactMap::iterator itr = _fixtureContactListeners.find(id);
	if(itr != _fixtureContactListeners.end()) {
		_fixtureContactListeners.erase(itr);
	}
}

b2Body *PhysicsEngine::createStaticBox(const Vector2 &pos, const Vector2 &dim) {
    b2BodyDef def;
    b2PolygonShape shape;
    b2Body *body;
    float halfWidth, halfHeight;
    
    halfWidth = dim.x / 2.0f;
    halfHeight = dim.y / 2.0f;

    // Create the body
    // The position passed is the center
    def.position.Set(pos.x, pos.y);
    body = _world->CreateBody(&def);
    
    // Create the fixture shape
    // The dimensions passed are the half-extents
    shape.SetAsBox(halfWidth, halfHeight);
    
    // Create the fixture
    body->CreateFixture(&shape, 0.0);

    return body;
}

b2Body *PhysicsEngine::createStaticChain(const std::vector<Vector2> &verts, bool loop) {
    b2BodyDef def;
    b2ChainShape shape;
    b2Body *body;
	b2Fixture *fixture;
    b2Vec2 *chainVectors;
    unsigned int i;
    unsigned int size;
    
    // Create the body
    // The position passed is the center
    body = _world->CreateBody(&def);
    
    // Create the fixture shape
    size = (unsigned int)verts.size();
    chainVectors = new b2Vec2[size];
    for(i = 0; i < size; i++) {
        chainVectors[i] = b2Vec2(verts[i].x, verts[i].y);
    }
    if(loop) {
        shape.CreateLoop(chainVectors, size);
    } else {
        shape.CreateChain(chainVectors, size);
    }
    
    // Create the fixture
    fixture = body->CreateFixture(&shape, 0.0);
    fixture->SetUserData((void*)&EdgeID);
    
    return body;
}

b2Body *PhysicsEngine::createDynamicBox(const Vector2 &pos, const Vector2 &dim, float density, float friction, bool canRotate) {
    b2BodyDef bDef;
    b2FixtureDef fDef;
    b2PolygonShape shape;
    b2Body *body;
    float halfWidth, halfHeight;
    
    halfWidth = dim.x / 2.0f;
    halfHeight = dim.y / 2.0f;
    
    // Create the body
    // Set the body as dynamic
    bDef.type = b2_dynamicBody;
    // Can this body rotate?
    if(!canRotate) {
        bDef.fixedRotation = true;
    }
    // The position passed is the center
    bDef.position.Set(pos.x, pos.y);
    body = _world->CreateBody(&bDef);
    
    // Create the fixture shape
    // The dimensions passed are the half-extents
    shape.SetAsBox(halfWidth, halfHeight);
    
    // Create the fixture
    fDef.shape = &shape;
    fDef.density = density;
    fDef.friction = friction;
    body->CreateFixture(&fDef);

    return body;
}
