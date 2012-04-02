#ifndef TEXT_H
#define TEXT_H

#include <UI/UIElement.h>

class Text: public UIElement {
public:
    Text(const std::string &name, const Vector2 &pos, const std::string &text, const std::string &font, const Vector2 &maxDims = Vector2(1.0f, 1.0f));
    void resize(int w, int h);

private:
    std::string _text;
    std::string _font;

    Vector2 _uiMaxDimensions;
};

#endif
