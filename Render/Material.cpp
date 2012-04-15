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

    // TODO - Only do this if UBOs are supported
    if(_ubo) { delete _ubo; }
    _ubo = _shader->createUniformBuffer("input_block");

    ShaderParamMap::iterator itr;
    for(itr = _shaderParams.begin(); itr != _shaderParams.end(); itr++) {
        if(itr->second->hasUniform()) {
            _ubo->setParameter(itr->first, itr->second->getUniformData());
        }
    }
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

    if(param->hasUniform()) {
        ASSERT(_ubo);
        _ubo->setParameter(name, param->getUniformData());
    }
}

void Material::enable() {
    ShaderParamMap::iterator itr;

    if(_shader) {
        _shader->enable();
    }
    
    if(_ubo) { _ubo->enable(); }

    for(itr = _shaderParams.begin(); itr != _shaderParams.end(); itr++) {
        if(itr->second->hasState()) { itr->second->enable(_shader); }
    }
}

void Material::disable() {
    ShaderParamMap::iterator itr;

    for(itr = _shaderParams.begin(); itr != _shaderParams.end(); itr++) {
        if(itr->second->hasState()) { itr->second->disable(_shader); }
    }
    
    if(_ubo) { _ubo->disable(); }

    if(_shader) {
        _shader->disable();
    }
}
