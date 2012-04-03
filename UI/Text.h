#ifndef TEXT_H
#define TEXT_H

#include <UI/UIElement.h>
#include <Render/Font.h>

class Text: public UIElement {
public:
    Text(const std::string &name, const Vector2 &pos, const Vector2 &dims, const std::string &text, const std::string &font, Font::Alignment textAlignment = Font::TOP_LEFT);
    void resize(int w, int h);

private:
    std::string _text;
    std::string _font;

    Font::Alignment _alignment;
};

#endif
