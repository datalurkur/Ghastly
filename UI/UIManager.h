#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <Engine/SceneManager.h>
#include <Render/ResizeListener.h>
#include <Engine/OrthoCamera.h>
#include <UI/UIElement.h>

class UIManager: public ResizeListener, public SceneManager {
public:
    UIManager();
    virtual ~UIManager();

    void render(RenderContext *context);
	void onResize(int w, int h);

    int getWidth() const;
    int getHeight() const;

    void addElement(UIElement *element);

private:
    int _width, _height;
    OrthoCamera *_camera;
    UIElement *_rootContainer;
};

#endif
