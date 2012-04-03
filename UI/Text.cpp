#include <UI/Text.h>
#include <UI/UIManager.h>
#include <Resource/TTFManager.h>

Text::Text(const std::string &name, const Vector2 &pos, const Vector2 &dims, const std::string &text, const std::string &font, Font::Alignment textAlignment):
	UIElement(name, pos, dims), _text(text), _font(font), _alignment(textAlignment) {}

void Text::resize(int width, int height) {
	UIElement::resize(width, height);
    clearRenderables();
    addRenderable(TTFManager::Get(_font)->createRenderable(_text, Vector2(_dimensions.x, _dimensions.y), _alignment));
}
