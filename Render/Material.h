#ifndef MATERIAL_H
#define MATERIAL_H

#include <Render/Texture.h>
#include <Base/Color.h>

class Material {
public:
    Material();
    virtual ~Material();

    void setColor(float r, float g, float b, float a);
    void setColor(const Color4 &color);
    void setTexture(Texture *texture);

    void enable();
    void disable();

private:
    Color4 _color;
    Texture *_texture;
};

#endif
