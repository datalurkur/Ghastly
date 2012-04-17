#include <UI/UIElement.h>
#include <Resource/MaterialManager.h>
#include <Resource/TTFManager.h>
#include <Render/Font.h>

UIElement::UIElement(const std::string &name, const Vector2 &pos, const Vector2 &dims):
    SceneNode(name), _uiPosition(pos), _uiDimensions(dims), _horizontalClamp(false), _verticalClamp(false) 
{
    int i;

    // Allow relative positions from the righthand side to be specified using negative numbers
    for(i = 0; i < 2; i++) {
        if(_uiPosition[i] < 0.0f) {
            _uiPosition[i] += 1.0f;

            // Swap the direction if the dimensions are negative
            if(_uiDimensions[i] < 0.0f) {
                _uiDimensions[i] = -_uiDimensions[i];
                _uiPosition[i] -= _uiDimensions[i];
            }
        }
    }
}

void UIElement::resize(int width, int height) {
    setPosition((_uiPosition.x - 0.5f) * width, (_uiPosition.y - 0.5f) * height, 0.0f);
    setDimensions((_uiDimensions.w * width), (_uiDimensions.h * height), 0.0f);

    NodeMap::iterator itr;
    for(itr = _children.begin(); itr != _children.end(); itr++) {
        ((UIElement*)itr->second)->resize(width, height);
    }
}
