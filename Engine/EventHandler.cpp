#include <Engine/EventHandler.h>

EventHandler::EventHandler() {
}

void EventHandler::handleEvents() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_VIDEORESIZE: {
				WindowListenerList::iterator itr = _windowListeners.begin();
				for(; itr != _windowListeners.end(); itr++) {
					(*itr)->resizeWindow(event.resize.w, event.resize.h);
				}
			} break;
			case SDL_QUIT: {
				WindowListenerList::iterator itr = _windowListeners.begin();
				for(; itr != _windowListeners.end(); itr++) {
					(*itr)->closeWindow();
				}
			} break;
            case SDL_KEYDOWN: {
                KeyboardEvent keyEvent(event.key.keysym.sym, event.key.keysym.mod);
                KeyboardListenerList::iterator itr = _keyboardListeners.begin();
                for (; itr != _keyboardListeners.end(); itr++) {
                    (*itr)->keyDown(&keyEvent);
                }
            } break;
            case SDL_KEYUP: {
                KeyboardEvent keyEvent(event.key.keysym.sym, event.key.keysym.mod);
                KeyboardListenerList::iterator itr = _keyboardListeners.begin();
                for (; itr != _keyboardListeners.end(); itr++) {
                    (*itr)->keyUp(&keyEvent);
                }
            } break;
		}
	}
}

void EventHandler::addWindowListener(WindowListener *listener) {
	_windowListeners.push_back(listener);
}

void EventHandler::addKeyboardListener(KeyboardListener *listener) {
	_keyboardListeners.push_back(listener);
}