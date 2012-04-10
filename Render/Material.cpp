#include <Render/Material.h>
#include <SDL/SDL_opengl.h>

Material::Material(): _color(1.0f, 1.0f, 1.0f, 1.0f), _texture(0), _shader(0) {
}

Material::~Material() {
}

void Material::setColor(float r, float g, float b, float a) {
    _color = Color4(r, g, b, a);
}

void Material::setColor(const Color4 &color) {
    _color = color;
}

void Material::setTexture(Texture *texture) {
    _texture = texture;
}

Texture *Material::getTexture() {
    return _texture;
}

void Material::setShader(Shader *shader) {
    _shader = shader;
}

Shader *Material::getShader() {
    return _shader;
}

void Material::enable() {
    if(_shader) {
        _shader->enable();
    }
    if(_texture) {
		_texture->enable();
    }
    glColor4f(_color.r, _color.g, _color.b, _color.a);
}

void Material::disable() {
	if(_texture) {
		_texture->disable();
	}
    if(_shader) {
        _shader->disable();
    }
}