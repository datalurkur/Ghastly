#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <Engine/SceneNode.h>

class UIManager;

class UIElement: public SceneNode<int> {
public:
    UIElement(const std::string &name, const Vec2f &pos, const Vec2f &dims = Vec2f(1.0f, 1.0f));
    virtual void resize(int width, int height);

    const Vec2f& getUIPosition() const;
    void setUIPosition(const Vec2f& pos, int width, int height);

protected:
    Vec2f _uiPosition, _uiDimensions;
    bool _horizontalClamp, _verticalClamp;
};

#endif
