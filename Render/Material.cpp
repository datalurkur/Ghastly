#include <Render/Material.h>
#include <SDL/SDL_opengl.h>

Material::Material(): _shader(0), _ubo(0) {
}

Material::~Material() {
    if(_ubo) { delete _ubo; }
}

void Material::setShader(Shader *shader) {
    _shader = shader;
    _ubo = _shader->createUniformBuffer();
}

Shader *Material::getShader() {
    return _shader;
}

void Material::setUniform(const std::string &name, const ShaderParameter &param) {
    ASSERT(_ubo);
    _ubo->setParameter(name, param.getUniformData());
}

void Material::enable() {
    if(_shader) {
        _shader->enable();
    }
}

void Material::disable() {
    if(_shader) {
        _shader->disable();
    }
}
