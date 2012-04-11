#ifndef TEXTUREPARAMETER_H
#define TEXTUREPARAMETER_H

#include <Render/Texture.h>

class TextureParameter {
public:
    TextureParameter(Texture *texture, GLuint frame = 0, GLuint channel = 0);

    void enable();
    void disable();

    const void* getUniformData() const;

private:
    Texture *_texture;
	GLuint _frame;
    GLuint _channel;
};

#endif