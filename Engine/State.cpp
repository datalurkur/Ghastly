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

void State::keyDown(KeyboardEvent *event) {
    Info("Not handling keydown event " << event->key() << ":" << event->modifier());
}

void State::keyUp(KeyboardEvent *event) {
    Info("Not handling keyup event " << event->key() << ":" << event->modifier());
}
