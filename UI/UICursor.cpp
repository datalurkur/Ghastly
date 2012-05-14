#include <UI/UICursor.h>
#include <Resource/MaterialManager.h>

UICursor::UICursor(const Vec2f &pos, const Vec2i &pixelSize): UIElement("cursor", pos), _pixelSize(pixelSize) {}

void UICursor::resize(int width, int height) {
    Renderable *renderable;

    UIElement::resize(width, height);
    clearRenderables();

    renderable = new Renderable();

    renderable->setMaterial(MaterialManager::Get("red"));

    // Vertex buffer
    float verts[3 * 3] = {
        0.0f, (float)-_pixelSize.y, 0.0f,
        (float)_pixelSize.x, (float)-_pixelSize.y * 0.8f, 0.0f,
        (float)_pixelSize.x * 0.15f, 0.0f, 0.0f
    };
    renderable->setAttribBuffer("position", 3, GL_FLOAT, 3, &verts[0]);

    // Index buffer
    unsigned int indices[3] = { 0, 1, 2 };
    renderable->setIndexBuffer(3, &indices[0]);

    addRenderable(renderable);
}

int UICursor::getCursorHeight() const { return _pixelSize.w; }

int UICursor::getCursorWidth() const { return _pixelSize.h; }