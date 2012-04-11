#ifndef SHADER_H
#define SHADER_H

#include <Base/Base.h>
#include <Render/UniformBuffer.h>

class Shader {
public:
    Shader();
    virtual ~Shader();

    GLuint compile(const char *shaderProgramData, GLenum type);
    void bindToUniformBlock(const std::string &uniformBlockName);

    void setup(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader);
    void teardown();

    UniformBuffer* createUniformBuffer();

    void enable();
    void disable();

protected:
    GLuint _program;
    GLuint _vertexShader, _geometryShader, _fragmentShader;

    bool _hasUniformBlock;
    std::string _uniformBlockName;

    friend class ShaderManager;
};

#endif
