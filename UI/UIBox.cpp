#include <UI/UIBox.h>
#include <Resource/MaterialManager.h>

UIBox::UIBox(const std::string &name, const Vec2f &pos, const Vec2f &dims, const std::string &material, float borderWidth, const std::string &borderMaterial):
    UIElement(name, pos, dims), _material(0), _uiBorder(borderWidth), _borderMaterial(0) {
    _material = MaterialManager::Get(material);
    if(_uiBorder != 0.0f) {
        _borderMaterial = MaterialManager::Get(borderMaterial);
    }
}

void UIBox::resize(int width, int height) {
    Renderable *renderable;
    float ratio = (width / (float)height);

    int bWidth  = (int)(_uiBorder * width),
        bHeight = (int)(_uiBorder * height * ratio);

    UIElement::resize(width, height);

    clearRenderables();

    // Add the center box
    if(_uiBorder > 0) {
        // Subtract space for the border if there is a border that protrudes inwards
        renderable = Renderable::OrthoBox(Vec2f((float)bWidth, (float)bHeight), Vec2f((float)(_dimensions.x - (bWidth * 2)), (float)(_dimensions.y - (bHeight * 2))), false, false, _material);
    } else {
        renderable = Renderable::OrthoBox(Vec2f(0.0f, 0.0f), Vec2f((float)_dimensions.x, (float)_dimensions.y), false, false, _material);
    }
    addRenderable(renderable);

    if(_uiBorder != 0) {
        int bXOffset = 0, bYOffset = 0;
        if(_uiBorder < 0) {
            bXOffset = bWidth;
            bYOffset = bHeight;
        }
        
        // Top border
        addRenderable(Renderable::OrthoBox(Vec2f((float)bWidth, (float)_dimensions.h), Vec2f((float)(_dimensions.w - (bWidth * 2)), (float)-bHeight), false, false, _borderMaterial));

        // Bottom border
        addRenderable(Renderable::OrthoBox(Vec2f((float)bWidth, 0.0f), Vec2f((float)(_dimensions.w - (bWidth * 2)), (float)bHeight), false, false, _borderMaterial));
        
        // Left border
        addRenderable(Renderable::OrthoBox(Vec2f((float)bWidth, 0.0f), Vec2f((float)-bWidth, (float)_dimensions.y), false, false, _borderMaterial));

        // Right border
        addRenderable(Renderable::OrthoBox(Vec2f((float)_dimensions.w, 0.0f), Vec2f((float)-bWidth, (float)_dimensions.y), false, false, _borderMaterial));
    }
}
