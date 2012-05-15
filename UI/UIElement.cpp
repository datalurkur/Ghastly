#include <UI/UIElement.h>
#include <Resource/MaterialManager.h>
#include <Resource/TTFManager.h>
#include <Render/Font.h>

UIElement::UIElement(const std::string &name, const Vec2f &pos, const Vec2f &dims):
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
    setPosition(Vec3i((int)(_uiPosition.x * width), (int)(_uiPosition.y * height), 0));
    setDimensions(Vec3i((int)(_uiDimensions.w * width), (int)(_uiDimensions.h * height), 0));

    NodeMap::iterator itr;
    for(itr = _children.begin(); itr != _children.end(); itr++) {
        ((UIElement*)itr->second)->resize(width, height);
    }
}

const Vec2f& UIElement::getUIPosition() const {
    return _uiPosition;
}

void UIElement::setUIPosition(const Vec2f& pos, int width, int height) {
    _uiPosition = pos;

    // FIXME
    // It's weird to have to pass in the dimensions
    // It's also weird to have this piece of code duplicated
    setPosition(Vec3i((int)(_uiPosition.x * width), (int)(_uiPosition.y * height), 0));
}