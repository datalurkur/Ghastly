#include <Engine/Mob.h>

const std::string Mob::NodeType = "Mob";

Mob::Mob(const std::string &name): Entity(name, NodeType) {}
Mob::Mob(const std::string &name, const std::string &type): Entity(name, type) {}

Mob::~Mob() {}