#include <UI/UIManager.h>
#include <Engine/OrthoCamera.h>

UIManager::UIManager(): _camera(0) {
    _camera = new OrthoCamera("UIManagerCamera");
    //_camera->setZoom(0.01f);
}

UIManager::~UIManager() {
    if(_camera) { delete _camera; }
}

void UIManager::render(RenderContext *context) {
    SceneManager::render(_camera, context);
}

void UIManager::onResize(int w, int h) {
	//_camera->clampEdges(Vector2(0,0), Vector2(w,h));
	_camera->onResize(w, h);
}
