#ifndef KEYBOARDLISTENER_H
#define KEYBOARDLISTENER_H

class KeyboardEvent {
public:
    KeyboardEvent(const int key, const int modifier);
    int key() const;
    int modifier() const;

private:
    int _key;
    int _modifier;
};

class KeyboardListener {
public:
    virtual bool keyDown(KeyboardEvent *event) = 0;
    virtual bool keyUp(KeyboardEvent *event) = 0;
};

#endif
