#include <Render/ShaderParameter.h>
#include <Base/Assertion.h>

ShaderParameter::ShaderParameter(GLenum type, unsigned int count, void *data):
{
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

    _data = malloc(totalByteSize);
    memcpy(_data, data, totalByteSize);
}

ShaderParameter::~ShaderParameter() {
    free(_data);
}
