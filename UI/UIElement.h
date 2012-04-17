#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <Engine/SceneNode.h>

class UIManager;

class UIElement: public SceneNode {
public:
    UIElement(const std::string &name, const Vector2 &pos, const Vector2 &dims = Vector2(1.0f, 1.0f));
    virtual void resize(int width, int height);

protected:
    Vector2 _uiPosition, _uiDimensions;
    bool _horizontalClamp, _verticalClamp;
};

#endif
