#ifndef KEYBOARDLISTENER_H
#define KEYBOARDLISTENER_H

class KeyboardEvent {
public:
    enum Keys {
        KEY_SPACE = 32,
        KEY_a     = 97,
        KEY_d     = 100,
        KEY_f     = 102,
        KEY_s     = 115,
        KEY_w     = 119,
        KEY_F1    = 282
    };

public:
    KeyboardEvent(const int key, const int modifier);
    int key() const;
    int modifier() const;

private:
    int _key, _modifier;
};

class KeyboardListener {
public:
	KeyboardListener();

    virtual void keyDown(KeyboardEvent *event) = 0;
    virtual void keyUp(KeyboardEvent *event) = 0;
};

#endif
