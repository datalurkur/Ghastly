#ifndef UIBUTTON_H
#define UIBUTTON_H

#include <UI/UIBox.h>

class UIButton: public UIBox {
public:
    UIButton(const std::string &name, const Vector2 &pos, const Vector2 &dims, const std::string &material, float borderWidth = 0.0f, const std::string &borderMaterial = "");

private:
};

#endif
