#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <UI/UIElement.h>
#include <Engine/SceneNode.h>
#include <Engine/OrthoCamera.h>
#include <Render/ResizeListener.h>
#include <Render/RenderContext.h>

class UIManager: public ResizeListener {
public:
    UIManager();
    virtual ~UIManager();

    void render(RenderContext *context);
    void update();

	void onResize(int w, int h);

    int getWidth() const;
    int getHeight() const;

    void addElement(UIElement *element);

private:
    int _width, _height;
    OrthoCamera *_camera;
    UIElement *_rootContainer;

    typedef std::list<SceneNode*> LayerList;
    LayerList _layers;
};

#endif
