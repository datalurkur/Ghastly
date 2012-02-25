#include <Engine/Entity.h>

const std::string Entity::NodeType = "Entity";

Entity::Entity(const std::string &name):
	SceneNode(name, NodeType), _physicsController(0)
{
}

Entity::Entity(const std::string &name, const std::string &type):
	SceneNode(name, type), _physicsController(0)
{
}

Entity::~Entity() {
    ControllerList::iterator itr = _controllers.begin();
    for(; itr != _controllers.end(); itr++) {
        delete (*itr);
    }
    _controllers.clear();
}

void Entity::update(int elapsed) {
    ControllerList::iterator itr = _controllers.begin();
    for(; itr != _controllers.end(); itr++) {
        (*itr)->update(elapsed);
    }
}

template <>
PhysicsController* Entity::addController<PhysicsController,PhysicsEngine>(PhysicsEngine* controlObject) {
    _physicsController = new PhysicsController(controlObject, this);
    _controllers.push_back(_physicsController);
    return _physicsController;
}