#ifndef UITEXT_H
#define UITEXT_H

#include <UI/UIElement.h>
#include <Render/Font.h>

class UIText: public UIElement {
public:
    UIText(const std::string &name, const Vec2f &pos, const Vec2f &dims, const std::string &text, const std::string &font, Font::Alignment textAlignment = Font::TOP_LEFT);
    void resize(int w, int h);

private:
    std::string _text;
    std::string _font;

    Font::Alignment _alignment;
};

#endif
