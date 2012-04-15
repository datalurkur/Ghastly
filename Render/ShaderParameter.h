#ifndef SHADERPARAMETER_H
#define SHADERPARAMETER_H

#include <Base/Base.h>
#include <Render/Shader.h>

class ShaderParameter {
public:
    ShaderParameter();
    virtual ~ShaderParameter();

    bool hasState() const;
    bool hasUniform() const;

    virtual void enable(Shader *shader);
    virtual void disable(Shader *shader);

    virtual const void* getUniformData() const;

protected:
    bool _hasState;
    bool _hasUniform;
};

#endif
