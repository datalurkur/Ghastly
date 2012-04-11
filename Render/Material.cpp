#include <Render/Material.h>
#include <SDL/SDL_opengl.h>

Material::Material(): _shader(0), _ubo(0) {
}

Material::~Material() {
    if(_ubo) { delete _ubo; }

    ShaderParamMap::iterator itr;
    for(itr = _shaderParams.begin(); itr != _shaderParams.end(); itr++ ) {
        delete itr->second;
    }
    _shaderParams.clear();
}

void Material::setShader(Shader *shader) {
    _shader = shader;
    _ubo = _shader->createUniformBuffer();
}

Shader *Material::getShader() {
    return _shader;
}

void Material::setParameter(const std::string &name, ShaderParameter *param) {
    ShaderParamMap::iterator itr;
    itr = _shaderParams.find(name);

    if(itr != _shaderParams.end()) {
        delete itr->second;
    }

    if(param) {
        _shaderParams[name] = param;
    } else if(itr != _shaderParams.end()) {
        _shaderParams.erase(itr);
    }

    ASSERT(_ubo);
    _ubo->setParameter(name, param->getUniformData());
}

void Material::enable() {
    ShaderParamMap::iterator itr;

    if(_shader) {
        _shader->enable();
    }

    for(itr = _shaderParams.begin(); itr != _shaderParams.end(); itr++) {
        if(itr->second->hasState()) { itr->second->enable(); }
    }
}

void Material::disable() {
    ShaderParamMap::iterator itr;

    for(itr = _shaderParams.begin(); itr != _shaderParams.end(); itr++) {
        if(itr->second->hasState()) { itr->second->disable(); }
    }

    if(_shader) {
        _shader->disable();
    }
}
