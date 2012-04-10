#include <Render/ShaderParameter.h>
#include <Base/Assertion.h>

ShaderParameter::ShaderParameter(const std::string &parameterName):
    _parameterName(parameterName), _data(0)
{
}

ShaderParameter::~ShaderParameter() {
    if(_data) { free(_data); }
}

void ShaderParameter::bind(Shader *shader) {
    _parameterLocation = shader->getUniformLocation(_parameterName);
}

void ShaderParameter::setData(GLenum type, unsigned int count, void *data) {
    unsigned int totalByteSize;

    _type = type;
    _count = count;

    switch(type) {
    case GL_FLOAT:
        totalByteSize = sizeof(float) * count;
        break;
    case GL_INT:
        totalByteSize = sizeof(int) * count;
        break;
    case GL_BOOL:
        totalByteSize = sizeof(bool) * count;
        break;
    default:
        ASSERT(0);
        break;
    };

    if(_data) { free(_data); }
    _data = malloc(totalByteSize);
    memcpy(_data, data, totalByteSize);
}
