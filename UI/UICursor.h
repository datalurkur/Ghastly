#ifndef UICURSOR_H
#define UICURSOR_H

#include <UI/UIElement.h>

class UICursor: public UIElement {
public:
    UICursor(const Vec2f &pos, const Vec2i &pixelSize = Vec2i(10,20));
    void resize(int width, int height);

    int getCursorHeight() const;
    int getCursorWidth() const;

private:
    Vec2i _pixelSize;
};

#endif