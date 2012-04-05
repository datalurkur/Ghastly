#include <UI/UIButton.h>
#include <Resource/MaterialManager.h>

UIButton::UIButton(const std::string &name, const Vector2 &pos, const Vector2 &dims, const std::string &material, float borderWidth, const std::string &borderMaterial):
    UIBox(name, pos, dims, material, borderWidth, borderMaterial) {
}
