#ifndef MOUSELISTENER_H
#define MOUSELISTENER_H

class MouseMovedEvent {
public:
    MouseMovedEvent(int absoluteX, int absoluteY, int relativeX, int relativeY);

    int absoluteX() const;
    int absoluteY() const;
    int relativeX() const;
    int relativeY() const;

private:
    int _absoluteX;
    int _absoluteY;
    int _relativeX;
    int _relativeY;
};

class MouseButtonEvent {
public:
    MouseButtonEvent(int button, bool pressed);

    int button() const;
    bool pressed() const;

private:
    int  _button;
    bool _pressed;
};

class MouseListener {
public:
    virtual bool mouseMoved(MouseMovedEvent *event);
    virtual bool mouseButton(MouseButtonEvent *event);
};

#endif