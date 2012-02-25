#include <Base/Log.h>
#include <Engine/PhysicsController.h>

PhysicsController::PhysicsController(PhysicsEngine *engine, SceneNode *node):
    Controller(node), _engine(engine), _body(0), _updates(true)
{}

PhysicsController::~PhysicsController() {
    // We assume that the controller is torn down by the entity teardown,
    //  which should in turn remove the physics body from the physics simulation
    _engine->destroyObject(_body);
}

void PhysicsController::update(int elapsed) {
    if(_updates) {
        b2Vec2 pos = _body->GetPosition();
        //Info("Updating position of " << _node->getName() << " to " << pos.x << "," << pos.y);
        _node->setPosition(pos.x, pos.y, 0);
    }
}

void PhysicsController::updatePosition(const Vector2 &pos) {
    _body->SetTransform(b2Vec2(pos.x, pos.y), _body->GetAngle());
}

void PhysicsController::setBody(b2Body *body) {
    if(_body) {
        _engine->destroyObject(_body);
    }
    _body = body;
}

b2Body *PhysicsController::getBody() {
	return _body;
}

void PhysicsController::disallowUpdates() { _updates = false; }
void PhysicsController::allowUpdates() { _updates = true; }

PhysicsEngine *PhysicsController::getEngine() { return _engine; }
