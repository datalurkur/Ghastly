#ifndef UILISTENER_H
#define UILISTENER_H

class UIListener {
public:
    enum ButtonState {
        PRESSED = 0,
        RELEASED,
        ENTER,
        LEAVE
    };

public:
    virtual void buttonStateChange(const std::string &buttonName, ButtonState state) {}
};

#endif
