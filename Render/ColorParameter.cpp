#include <Render/ColorParameter.h>
#include <Base/Assertion.h>

ColorParameter::ColorParameter(const Color4 &color): _color(color) {
}

const void* ColorParameter::getUniformData() const {
    return _color.ptr();
}
