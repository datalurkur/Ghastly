#include <UI/UIBox.h>
#include <Resource/MaterialManager.h>

UIBox::UIBox(const std::string &name, const Vector2 &pos, const Vector2 &dims, const std::string &material, float borderWidth, const std::string &borderMaterial):
    UIElement(name, pos, dims), _material(0), _uiBorder(borderWidth), _borderMaterial(0) {
    _material = MaterialManager::Get(material);
    if(_uiBorder != 0.0f) {
        _borderMaterial = MaterialManager::Get(borderMaterial);
    }
}

void UIBox::resize(int width, int height) {
    Renderable *renderable;
	float ratio = (width / (float)height);

    float bWidth  = (_uiBorder * width),
          bHeight = (_uiBorder * height * ratio);

    UIElement::resize(width, height);

    clearRenderables();

    // Add the center box
    if(_uiBorder > 0.0f) {
        // Subtract space for the border if there is a border that protrudes inwards
        renderable = Renderable::OrthoBox(Vector3(bWidth, bHeight, 0.0f), Vector2(_dimensions.x - (bWidth * 2), _dimensions.y - (bHeight * 2)), false, false, _material);
    } else {
        renderable = Renderable::OrthoBox(Vector3(0.0f, 0.0f, 0.0f), Vector2(_dimensions.x, _dimensions.y), false, false, _material);
    }
    addRenderable(renderable);

    if(_uiBorder != 0.0f) {
        float bXOffset = 0.0f, bYOffset = 0.0f;
        if(_uiBorder < 0.0f) {
            bXOffset = bWidth;
            bYOffset = bHeight;
        }
		
        // Top border
        addRenderable(Renderable::OrthoBox(Vector3(bWidth, _dimensions.h, 0.0f), Vector2(_dimensions.w - (bWidth * 2), -bHeight), false, false, _borderMaterial));

        // Bottom border
        addRenderable(Renderable::OrthoBox(Vector3(bWidth, 0.0f, 0.0f), Vector2(_dimensions.w - (bWidth * 2), bHeight), false, false, _borderMaterial));
		
        // Left border
        addRenderable(Renderable::OrthoBox(Vector3(bWidth, 0.0f, 0.0f), Vector2(-bWidth, _dimensions.y), false, false, _borderMaterial));

        // Right border
        addRenderable(Renderable::OrthoBox(Vector3(_dimensions.w, 0.0f, 0.0f), Vector2(-bWidth, _dimensions.y), false, false, _borderMaterial));
    }
}
