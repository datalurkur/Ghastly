#include <UI/UIElement.h>
#include <Resource/MaterialManager.h>
#include <Resource/TTFManager.h>
#include <Render/Font.h>

UIElement::UIElement(const std::string &name, const Vector2 &pos): SceneNode(name), _uiPosition(pos) {}

void UIElement::resize(int width, int height) {
    setPosition((_uiPosition.x - 0.5f) * height, (_uiPosition.y - 0.5f) * height, 0.0f);
    
    NodeMap::iterator itr;
    for(itr = _children.begin(); itr != _children.end(); itr++) {
        ((UIElement*)itr->second)->resize(width, height);
    }
}
