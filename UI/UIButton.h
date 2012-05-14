#ifndef UIBUTTON_H
#define UIBUTTON_H

#include <UI/UIBox.h>

class UIButton: public UIBox {
public:
    UIButton(const std::string &name, const Vec2f &pos, const Vec2f &dims, const std::string &material, float borderWidth = 0.0f, const std::string &borderMaterial = "");

private:
};

#endif
