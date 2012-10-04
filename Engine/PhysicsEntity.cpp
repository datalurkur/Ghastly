#include <Engine/PhysicsEntity.h>

const std::string PhysicsEntity::NodeType = "PhysicsEntity";

PhysicsEntity::PhysicsEntity(const std::string &name):
    Entity(name, NodeType), _physicsController(0)
{}

PhysicsEntity::PhysicsEntity(const std::string &name, const std::string &type):
    Entity(name, type), _physicsController(0)
{}

PhysicsEntity::~PhysicsEntity() {}

template <>
PhysicsController* PhysicsEntity::addController<PhysicsController,PhysicsEngine>(PhysicsEngine* controlObject) {
    _physicsController = new PhysicsController(controlObject, this);
    _controllers.push_back(_physicsController);
    return _physicsController;
}