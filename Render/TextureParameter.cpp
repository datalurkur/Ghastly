#include <Render/TextureParameter.h>
#include <Base/Assertion.h>

TextureParameter::TextureParameter(Texture *texture, GLuint frame, GLuint channel):
    _texture(texture), _frame(frame), _channel(channel)
{
    // Multitexturing is not yet supported
    ASSERT(_channel == 0);
}

void TextureParameter::enable() {
    _texture->enable(_frame);
}

void TextureParameter::disable() {
    _texture->disable();
}

const void* TextureParameter::getUniformData() const {
    return (void*)&(_texture->_ids[_frame]);
}
