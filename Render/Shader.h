#ifndef SHADER_H
#define SHADER_H

#include <Base/Base.h>
#include <Render/UniformBuffer.h>

class Shader {
public:
    Shader();
    virtual ~Shader();

    GLuint compile(const char *shaderProgramData, GLenum type);
    GLint getUniformLocation(const std::string &uniformName);

    void setup(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader);
    void teardown();

    UniformBuffer* createUniformBuffer(const std::string &uniformBlockName);

    void enable();
    void disable();

protected:
    GLuint _program;
    GLuint _vertexShader, _geometryShader, _fragmentShader;

    friend class ShaderManager;
};

#endif
