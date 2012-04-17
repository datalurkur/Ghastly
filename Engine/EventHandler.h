#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <Base/Base.h>
#include <Engine/WindowListener.h>
#include <Engine/KeyboardListener.h>

class EventHandler {
public:
    EventHandler(int windowID);

    void handleEvents();

    void addWindowListener(WindowListener *listener);
    void addKeyboardListener(KeyboardListener *listener);

private:
    typedef std::list<WindowListener*> WindowListenerList;
    WindowListenerList _windowListeners;
    typedef std::list<KeyboardListener*> KeyboardListenerList;
    KeyboardListenerList _keyboardListeners;

    int _windowID;
};

#endif
