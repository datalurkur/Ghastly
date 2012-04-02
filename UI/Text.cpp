#include <UI/Text.h>
#include <UI/UIManager.h>
#include <Resource/TTFManager.h>

Text::Text(const std::string &name, const Vector2 &pos, const std::string &text, const std::string &font): UIElement(name, pos), _text(text), _font(font) {}

void Text::resize(int width, int height) {
    UIElement::resize(width, height);
    clearRenderables();
    addRenderable(TTFManager::Get(_font)->createRenderable(_text));
}
