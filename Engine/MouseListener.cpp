#include <Engine/MouseListener.h>

MouseMovedEvent::MouseMovedEvent(int absoluteX, int absoluteY, int relativeX, int relativeY):
    _absoluteX(absoluteX), _absoluteY(absoluteY), _relativeX(relativeX), _relativeY(relativeY) {}
int MouseMovedEvent::absoluteX() const { return _absoluteX; }
int MouseMovedEvent::absoluteY() const { return _absoluteY; }
int MouseMovedEvent::relativeX() const { return _relativeX; }
int MouseMovedEvent::relativeY() const { return _relativeY; }

MouseButtonEvent::MouseButtonEvent(int button, bool pressed):
    _button(button), _pressed(pressed) {}
int MouseButtonEvent::button() const { return _button; }
bool MouseButtonEvent::pressed() const { return _pressed; }

bool MouseListener::mouseMoved(MouseMovedEvent *event) { return false; }
bool MouseListener::mouseButton(MouseButtonEvent *event) { return false; }
