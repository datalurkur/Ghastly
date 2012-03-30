#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <Engine/SceneNode.h>

class UIElement: public SceneNode {
public:
	static UIElement *Box(const Vector2 &pos, const Vector2 &dims, const Color4 &color, const std::string &name = "UIBox");
    static UIElement *DebugFont(const std::string &fontName, const std::string &name = "FontDebugger");
	static UIElement *Text(const Vector2 &pos, const std::string &text, const std::string &fontName, const std::string &name = "TextBox");

public:
	UIElement(const std::string &name);
};

#endif
