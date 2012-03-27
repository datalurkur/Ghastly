#include <UI/UIManager.h>
#include <Engine/OrthoCamera.h>

UIManager::UIManager(): _camera(0) {
    _camera = new OrthoCamera("UIManagerCamera");
}

UIManager::~UIManager() {
    if(_camera) { delete _camera; }
}

void UIManager::render(RenderContext *context) {
    SceneManager::render(_camera, context);
}
