#include <Render/ColorParameter.h>
#include <Base/Assertion.h>


ColorParameter::ColorParameter(const std::string &name, const Color4 &color):
    ShaderParameter(name), _color(color) {}
ColorParameter::ColorParameter(const std::string &blockName, const std::string &name, const Color4 &color):
    ShaderParameter(blockName, name), _color(color) {}

const void* ColorParameter::getUniformData() const {
    return _color.ptr();
}
