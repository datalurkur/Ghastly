#include <Render/Material.h>

Material::Material(): _shader(0) {
}

Material::~Material() {
    teardown();
}

void Material::teardown() {
    UniformBufferMap::iterator uboItr;
    for(uboItr = _ubos.begin(); uboItr != _ubos.end(); uboItr++) {
        delete uboItr->second;
    }
    _ubos.clear();
    
    ShaderParamMap::iterator paramItr;
    for(paramItr = _shaderParams.begin(); paramItr != _shaderParams.end(); paramItr++ ) {
        delete paramItr->second;
    }
    _shaderParams.clear();
}

void Material::setShader(Shader *shader) {
    teardown();
    _shader = shader;
}
Shader *Material::getShader() { return _shader; }

void Material::setParameter(ShaderParameter *param) {
    if(param->hasUniform()) {
        ShaderParamMap::iterator paramItr;

        paramItr = _shaderParams.find(param->getUniformName());
        if(paramItr != _shaderParams.end()) {
            delete paramItr->second;
        }
        
        if(param) {
            _shaderParams[param->getUniformName()] = param;
        } else {
            _shaderParams.erase(paramItr);
        }

        if(param->isBlockUniform()) {
            UniformBufferMap::iterator uboItr;
            
            uboItr = _ubos.find(param->getBlockName());
            if(uboItr == _ubos.end()) {
                _ubos[param->getBlockName()] = new UniformBuffer(_shader, param->getBlockName());
            }
            
            _ubos[param->getBlockName()]->setParameter(param->getUniformName(), param->getUniformData());
        }
    } else {
        Info("Parameters must have names.");
        ASSERT(0);
    }
}

void Material::enable() {
    ShaderParamMap::iterator paramItr;
    UniformBufferMap::iterator uboItr;
    
    if(_shader) {
        _shader->enable();
    }

    for(paramItr = _shaderParams.begin(); paramItr != _shaderParams.end(); paramItr++) {
        if(paramItr->second->hasState()) { paramItr->second->enable(_shader); }
    }
    for(uboItr = _ubos.begin(); uboItr != _ubos.end(); uboItr++) {
        uboItr->second->enable();
    }
}

void Material::disable() {
    ShaderParamMap::iterator paramItr;
    UniformBufferMap::iterator uboItr;

    for(paramItr = _shaderParams.begin(); paramItr != _shaderParams.end(); paramItr++) {
        if(paramItr->second->hasState()) { paramItr->second->disable(_shader); }
    }
    for(uboItr = _ubos.begin(); uboItr != _ubos.end(); uboItr++) {
        uboItr->second->disable();
    }

    if(_shader) {
        _shader->disable();
    }
}
