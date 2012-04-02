#include <UI/UIBox.h>
#include <Resource/MaterialManager.h>

UIBox::UIBox(const std::string &name, const Vector2 &pos, const Vector2 &dims, const std::string &material): UIElement(name, pos), _uiDimensions(dims) {
    _material = MaterialManager::Get(material);
}

void UIBox::resize(int width, int height) {
    UIElement::resize(width, height);
    setDimensions(_uiDimensions.w * width, _uiDimensions.h * height, 0.0f);
    
    clearRenderables();
    Renderable *boxRenderable = Renderable::OrthoBox(Vector3(0.0f, 0.0f, 0.0f), _dimensions, false, false);
    boxRenderable->setMaterial(_material);
    addRenderable(boxRenderable);
}