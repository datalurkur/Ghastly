#include <Engine/State.h>

State::State(): _parent(0) {
}

State::~State() {
}

void State::setParent(ParentState *parent) {
    ASSERT(parent == 0 || _parent == 0);
    _parent = parent;
}

void State::setCore(Core *core) {
    _core = core;
}

bool State::keyDown(KeyboardEvent *event) {
    Debug("Not handling keydown event " << event->key() << ":" << event->modifier());
    return false;
}

bool State::keyUp(KeyboardEvent *event) {
    Debug("Not handling keyup event " << event->key() << ":" << event->modifier());
    return false;
}

bool State::mouseMoved(MouseMovedEvent *event) {
    //Debug("Not handling mouse movement event");
    return false;
}

bool State::mouseButton(MouseButtonEvent *event) {
    Debug("Not handling mouse button event");
    return false;
}