#ifndef TEXTUREPARAMETER_H
#define TEXTUREPARAMETER_H

#include <Render/ShaderParameter.h>
#include <Render/Texture.h>

class TextureParameter: public ShaderParameter {
public:
    TextureParameter(const std::string &name, Texture *texture, GLuint frame = 0, GLuint channel = 0);
    TextureParameter(const std::string &blockName, const std::string &name, Texture *texture, GLuint frame = 0, GLuint channel = 0);

    void enable(Shader *shader);
    void disable(Shader *shader);

private:
    Texture *_texture;
    GLuint _frame;
    GLuint _channel;
};

#endif