#include <UI/UIButton.h>
#include <Resource/MaterialManager.h>

UIButton::UIButton(const std::string &name, const Vec2f &pos, const Vec2f &dims, const std::string &material, float borderWidth, const std::string &borderMaterial):
    UIBox(name, pos, dims, material, borderWidth, borderMaterial) {
}

void UIButton::onHover() {
    Info("Button " << _name << " hovered");
}

void UIButton::onLeave() {
    Info("Button " << _name << " left");
}