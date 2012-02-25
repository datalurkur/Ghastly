#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Base/Base.h>
#include <Engine/SceneNode.h>

class Controller {
public:
    Controller(SceneNode *node);
    virtual ~Controller();

    virtual void update(int elapsed) = 0;

protected:
    SceneNode* _node;
};

typedef std::list<Controller*> ControllerList;

#endif