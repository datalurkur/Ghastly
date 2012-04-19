#ifndef COLORPARAMETER_H
#define COLORPARAMETER_H

#include <Render/ShaderParameter.h>
#include <Base/Color.h>

class ColorParameter: public ShaderParameter {
public:
    ColorParameter(const std::string &name, const Color4 &color);
    ColorParameter(const std::string &blockName, const std::string &name, const Color4 &color);
    const void* getUniformData() const;

private:
    Color4 _color;
};

#endif
