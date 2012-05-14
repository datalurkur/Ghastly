#ifndef PARENTSTATE_H
#define PARENTSTATE_H

#include <Engine/State.h>

class ParentState: public State {
public:
    ParentState();
    virtual ~ParentState();

    void setState(State *state, ...);
    void setState(State *state, va_list args);
    void pushState(State *state, ...);
    void pushState(State *state, va_list args);
    bool popState(bool deleteOnPop = true);
    State *activeState();

    void flushStates();

    virtual bool keyDown(KeyboardEvent *event);
    virtual bool keyUp(KeyboardEvent *event);
    virtual bool mouseMoved(MouseMovedEvent *event);
    virtual bool mouseButton(MouseButtonEvent *event);

    virtual bool update(int elapsed);
    virtual bool render(RenderContext *renderContext);

protected:
    virtual void setup(va_list args);
    virtual void teardown();

private:
    std::stack <State*> _stateStack;
};

#endif
