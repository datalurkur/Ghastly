#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <Engine/SceneManager.h>
#include <Render/ResizeListener.h>
#include <Engine/OrthoCamera.h>

class UIManager: public ResizeListener, public SceneManager {
public:
    UIManager();
    virtual ~UIManager();

    void render(RenderContext *context);
	void onResize(int w, int h);

private:
    OrthoCamera *_camera;
};

#endif
