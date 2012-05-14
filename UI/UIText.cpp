#include <UI/UIText.h>
#include <UI/UIManager.h>
#include <Resource/TTFManager.h>

UIText::UIText(const std::string &name, const Vec2f &pos, const Vec2f &dims, const std::string &text, const std::string &font, Font::Alignment textAlignment):
    UIElement(name, pos, dims), _text(text), _font(font), _alignment(textAlignment) {}

void UIText::resize(int width, int height) {
    UIElement::resize(width, height);
    clearRenderables();
    addRenderable(TTFManager::Get(_font)->createRenderable(_text, Vec2i(_dimensions.x, _dimensions.y), _alignment));
}
