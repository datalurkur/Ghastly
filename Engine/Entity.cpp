#include <Engine/Entity.h>

const std::string Entity::NodeType = "Entity";

Entity::Entity(const std::string &name):
    SceneNode(name, NodeType)
{
}

Entity::Entity(const std::string &name, const std::string &type):
    SceneNode(name, type)
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