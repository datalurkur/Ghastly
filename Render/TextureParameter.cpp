#include <Render/TextureParameter.h>
#include <Base/Assertion.h>

TextureParameter::TextureParameter(Texture *texture, GLuint frame, GLuint channel): _texture(texture), _frame(frame), _channel(channel) {
    // TODO - Add multitexturing
    // Multitexturing is not yet supported
    ASSERT(_channel == 0);

    // sampler2D types cannot be included in uniform buffer objects, so just pass it in manually
    _hasState = true;
}

void TextureParameter::enable(Shader *shader) {
    GLint location;

    _texture->enable(_frame);

    // TODO - Add the ability to specify the uniform name (multitexturing, FBOs, etc)
    location = shader->getUniformLocation("texture0");
    glUniform1i(location, _channel);
}

void TextureParameter::disable(Shader *shader) {
    _texture->disable();
}
