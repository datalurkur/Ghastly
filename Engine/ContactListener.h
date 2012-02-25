#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H

#include <Base/Base.h>

class Entity;
typedef const char FixtureID;

class ContactListener {
public:
    ContactListener();

    virtual void contactBegins(Entity *a, Entity *b) = 0;
	virtual void contactBegins(FixtureID *trigger, FixtureID *other) = 0;
    virtual void contactEnds(Entity *a, Entity *b) = 0;
	virtual void contactEnds(FixtureID *trigger, FixtureID *other) = 0;

private:
};

#endif