#ifndef UIBOX_H
#define UIBOX_H

#include <UI/UIElement.h>

class UIBox: public UIElement {
public:
    UIBox(const std::string &name, const Vec2f &pos, const Vec2f &dims, const std::string &material, float borderWidth = 0.0f, const std::string &borderMaterial = "");
    void resize(int width, int height);

private:
    Vec2f _uiDimensions;
    Material *_material;

    float _uiBorder;
    Material *_borderMaterial;
};

#endif
