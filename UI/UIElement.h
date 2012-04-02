#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <Engine/SceneNode.h>

class UIManager;

class UIElement: public SceneNode {
public:
	UIElement(const std::string &name, const Vector2 &pos);
    virtual void resize(int width, int height);

protected:
    Vector2 _uiPosition;
};

#endif
