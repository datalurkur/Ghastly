#ifndef STATE_H
#define STATE_H

#include <Base/Log.h>
#include <Engine/KeyboardListener.h>
#include <Render/RenderContext.h>

class ParentState;
class Core;

class State: public KeyboardListener {
public:
    State();
    virtual ~State();

    virtual void update(int elapsed) = 0;
    virtual void render(RenderContext *renderContext) = 0;

    virtual void keyDown(KeyboardEvent *event);
    virtual void keyUp(KeyboardEvent *event);

protected:
    virtual void setup(va_list args) = 0;
    virtual void teardown() = 0;

    void setParent(ParentState *parent);
    void setCore(Core *core);

protected:
	friend class ParentState;
    ParentState *_parent;
    Core *_core;
};

#endif
