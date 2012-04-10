#include <Render/Material.h>
#include <SDL/SDL_opengl.h>

Material::Material(): _shader(0) {
}

Material::~Material() {
}

void Material::setShader(Shader *shader) {
    _shader = shader;
}

Shader *Material::getShader() {
    return _shader;
}

void Material::setUniform(const std::string &name, ShaderParameter *param) {
    ShaderParamMap::iterator itr;
    itr = _shaderUniforms.find(name);
    if(itr != _shaderUniforms.end()) {
        delete itr->second;
    }
    _shaderUniforms[name] = param;
}

ShaderParameter* Material::getUniform(const std::string &name) {
    ShaderParamMap::iterator itr;
    itr = _shaderUniforms.find(name);

    if(itr != _shaderUniforms.end()) {
        return itr->second;
    } else {
        return 0;
    }
}

void Material::enable() {
    if(_shader) {
        _shader->enable();
    }

    ShaderParamList::iterator itr;
    for(itr = _shaderUniforms.begin(); itr != _shaderUniforms.end(); itr++ ) {
        ASSERT(0);
    }
}

void Material::disable() {
    if(_shader) {
        _shader->disable();
    }
}
