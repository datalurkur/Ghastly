#include <UI/UIManager.h>
#include <Engine/OrthoCamera.h>

UIManager::UIManager(): _camera(0) {
    _camera = new OrthoCamera("UIManagerCamera");

    UIElement *root = new UIElement("root", Vector2(0.5f, 0.5f));
    _layers.push_back(root);
    root->updateCachedValues();
}

UIManager::~UIManager() {
    if(_camera) { delete _camera; }

    LayerList::iterator itr;
    for(itr = _layers.begin(); itr != _layers.end(); itr++) {
        delete (*itr);
    }
}

void UIManager::render(RenderContext *context) {
    LayerList::reverse_iterator layerItr;

    glDisable(GL_DEPTH_TEST);

    // Render the layers from bottom to top
    for(layerItr = _layers.rbegin(); layerItr != _layers.rend(); layerItr++) {
        NodeList visibleNodes;
        RenderableList renderables;

        (*layerItr)->getNodes(visibleNodes, _camera);

        NodeList::iterator nodeItr;
        for(nodeItr = visibleNodes.begin(); nodeItr != visibleNodes.end(); nodeItr++) {
            (*nodeItr)->getRenderables(renderables);
        }

        _camera->setup();
        context->render(_camera->getProjection(), _camera->getModelView(), renderables);
    }
    
    glEnable(GL_DEPTH_TEST);
}

void UIManager::update() {
    LayerList::iterator itr;
    for(itr = _layers.begin(); itr != _layers.end(); itr++) {
        (*itr)->updateCachedValues();
    }
}

void UIManager::onResize(int w, int h) {
    _width = w;
    _height = h;
	_camera->clampEdges(Vector2(-(w/2.0f),-(h/2.0f)), Vector2((w/2.0f),(h/2.0f)));

    LayerList::iterator itr;
    for(itr = _layers.begin(); itr != _layers.end(); itr++) {
        ((UIElement*)(*itr))->resize(w, h);
    }
}

int UIManager::getWidth() const { return _width; }
int UIManager::getHeight() const { return _height; }

void UIManager::addElement(UIElement *element) {
    element->resize(_width, _height);
    _layers.front()->addChild(element);
    _layers.front()->updateCachedValues();
}
