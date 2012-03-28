#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <Engine/SceneNode.h>

class UIElement: public SceneNode {
public:
	static UIElement *Box(const Vector2 &pos, const Vector2 &dims, const Color4 &color, const std::string &name = "UIBox");

public:
	UIElement(const std::string &name);
};

#endif
