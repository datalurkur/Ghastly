#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <Base/Base.h>
#include <Engine/WindowListener.h>
#include <Engine/KeyboardListener.h>
#include <Engine/MouseListener.h>

class EventHandler {
public:
    EventHandler(unsigned int windowID);

    void handleEvents();

    void addWindowListener(WindowListener *listener);
    void addKeyboardListener(KeyboardListener *listener);
    void addMouseListener(MouseListener *listener);

private:
    typedef std::list<WindowListener*> WindowListenerList;
    WindowListenerList _windowListeners;
    typedef std::list<KeyboardListener*> KeyboardListenerList;
    KeyboardListenerList _keyboardListeners;
    typedef std::list<MouseListener*> MouseListenerList;
    MouseListenerList _mouseListeners;

    unsigned int _windowID;
};

#endif
