#ifndef SHADER_H
#define SHADER_H

#include <SDL/SDL_opengl.h>

class Shader {
public:
    Shader();
    virtual ~Shader();

    GLuint compile(const char *shaderProgramData, GLenum type);
    void setup(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader);
    void teardown();

    void enable();
    void disable();

    GLint getAttributeLocation(const std::string &attributeName);
    GLint getUniformLocation(const std::string &uniformName);

protected:
    GLuint _program;
    GLuint _vertexShader, _geometryShader, _fragmentShader;

    friend class ShaderManager;
};

#endif
