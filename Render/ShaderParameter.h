#ifndef SHADERPARAMETER_H
#define SHADERPARAMETER_H

#include <Base/Base.h>
#include <Render/Shader.h>

class ShaderParameter {
public:
    ShaderParameter(bool hasState = false);
    ShaderParameter(const std::string &uniformName, bool hasState = false);
    ShaderParameter(const std::string &blockName, const std::string &uniformName, bool hasState = false);
    virtual ~ShaderParameter();

    bool hasState() const;
    bool hasUniform() const;
    bool isBlockUniform() const;
    const std::string &getUniformName() const;
    const std::string &getBlockName() const;

    virtual void enable(Shader *shader);
    virtual void disable(Shader *shader);

    virtual const void* getUniformData() const;

protected:
    bool _hasState;

    bool _hasUniform;
    std::string _uniformName;
                       
    bool _blockUniform;
    std::string _blockName;
};

#endif
