#ifndef MOB_H
#define MOB_H

#include <Engine/Entity.h>

// Mobile Entity (monster, player, etc)
//  This is designed to be subclassed, not used directly

class Mob: public Entity {
public:
	static const std::string NodeType;
    
public:
    Mob(const std::string &name);
	virtual ~Mob();

protected:
    Mob(const std::string &name, const std::string &type);

protected:
    float _maxSpeed, _accel;
};

#endif