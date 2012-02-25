#include <Engine/ParentState.h>

ParentState::ParentState() {
}

ParentState::~ParentState() {
    teardown();
}

void ParentState::setup(va_list args) {
}

void ParentState::teardown() {
    flushStates();
}

void ParentState::setState(State *state, ...) {
    va_list args;
    va_start(args, state);
    setState(state, args);
    va_end(args);
}

void ParentState::setState(State *state, va_list args) {
    flushStates();
    pushState(state, args);
}

void ParentState::pushState(State *state, ...) {
    va_list args;
    va_start(args, state);
    pushState(state, args);
    va_end(args);
}

void ParentState::pushState(State *state, va_list args) {
    state->setParent(this);
    state->setCore(_core);
    state->setup(args);
    _stateStack.push(state);
}

bool ParentState::popState(bool deleteOnPop = true) {
	if(_stateStack.empty()) {
		return false;
	} else {
		State *state = _stateStack.top();
        _stateStack.pop();
		if(state) {
			state->teardown();
			if(deleteOnPop) {
				delete state;
			}
		}
        return true;
    }
}

State* ParentState::activeState() {
    if(_stateStack.empty()) {
        return NULL;
    } else {
        return _stateStack.top();
    }
}

void ParentState::flushStates() {
    while(popState()) {}
}

void ParentState::keyDown(KeyboardEvent *event) {
    if(activeState()) {
        activeState()->keyDown(event);
    }
}

void ParentState::keyUp(KeyboardEvent *event) {
    if(activeState()) {
        activeState()->keyUp(event);
    }
}

void ParentState::update(int elapsed) {
    if(activeState()) {
        activeState()->update(elapsed);
    }
}

void ParentState::render(RenderContext *renderContext) {
    if(activeState()) {
        activeState()->render(renderContext);
    }
}
