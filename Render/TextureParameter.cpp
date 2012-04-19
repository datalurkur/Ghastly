#include <Render/TextureParameter.h>
#include <Base/Assertion.h>

TextureParameter::TextureParameter(const std::string &name, Texture *texture, GLuint frame, GLuint channel):
    ShaderParameter(name, true), _texture(texture), _frame(frame), _channel(channel) {}

TextureParameter::TextureParameter(const std::string &blockName, const std::string &name, Texture *texture, GLuint frame, GLuint channel):
    ShaderParameter(blockName, name, true), _texture(texture), _frame(frame), _channel(channel) {}

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
