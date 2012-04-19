#include <Engine/EventHandler.h>
#include <Base/Log.h>

EventHandler::EventHandler(unsigned int windowID): _windowID(windowID) {}

void EventHandler::handleEvents() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_WINDOWEVENT: {
                if(event.window.windowID == _windowID) {
                    switch(event.window.event) {
                        case SDL_WINDOWEVENT_RESIZED:
                        case SDL_WINDOWEVENT_SIZE_CHANGED: {
                            WindowListenerList::iterator itr = _windowListeners.begin();
                            for(; itr != _windowListeners.end(); itr++) {
                                (*itr)->resizeWindow(event.window.data1, event.window.data2);
                            }
                        } break;
                        case SDL_WINDOWEVENT_CLOSE: {
                            event.type = SDL_QUIT;
                            SDL_PushEvent(&event);
                        } break;
                    }
                }

            } break;
            case SDL_QUIT: {
                WindowListenerList::iterator itr = _windowListeners.begin();
                for(; itr != _windowListeners.end(); itr++) {
                    (*itr)->closeWindow();
                }
                SDL_Quit();
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
