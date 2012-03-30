#include <UI/UIElement.h>
#include <Resource/TTFManager.h>
#include <Render/Font.h>

UIElement* UIElement::Box(const Vector2 &pos, const Vector2 &dims, const Color4 &color, const std::string &name) {
	UIElement *element = new UIElement(name);
	element->setPosition(Vector3(pos.x, pos.y, 0.0f));
	element->setDimensions(Vector3(dims.x, dims.y, 0.0f));
	element->addRenderable(Renderable::OrthoBox(pos, dims, 0.0f, true, false));
	return element;
}

UIElement *UIElement::Text(const Vector2 &pos, const std::string &text, const std::string &fontName, const std::string &name) {
	Font *font = TTFManager::Get(fontName);
	UIElement *element = new UIElement(name);
	element->setPosition(Vector3(pos.x, pos.y, 0.0f));
	element->setDimensions(Vector3(font->textWidth(text), font->textHeight(text), 0.0f));
	element->addRenderable(font->createRenderable(text));
	return element;
}

UIElement::UIElement(const std::string &name): SceneNode(name) {}