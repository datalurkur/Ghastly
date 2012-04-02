#ifndef TEXT_H
#define TEXT_H

#include <UI/UIElement.h>

class Text: public UIElement {
public:
    Text(const std::string &name, const Vector2 &pos, const std::string &text, const std::string &font);
    void resize(int w, int h);
    
private:
    std::string _text;
    std::string _font;
};

#endif