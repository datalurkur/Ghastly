#ifndef SHADER_H
#define SHADER_H

#include <Base/Base.h>
#include <Render/UniformBuffer.h>

class Shader {
public:
    struct UniformInfo {
        GLuint index;
        GLint offset;
        GLint size;

        UniformInfo();
        UniformInfo(GLuint i, GLint o, GLint s);
        UniformInfo(const UniformInfo &other);
    };

public:
    Shader();
    virtual ~Shader();

    // Shader construction and destruction
    void setup(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader);
    void teardown();

    // Shader information accessors
    GLuint getUniformLocation(const std::string &uniformName);
    GLuint getAttribLocation(const std::string &attribName);
    GLuint getProgramHandle();
    const UniformInfo& getUniformInfo(const std::string &uniformBlockName, const std::string &uniformName);

    void enable();
    void disable();

protected:   
    GLuint compile(const char *shaderProgramData, GLenum type);
    bool fetchUniformBlockInformation(const std::string &uniformBlockName);

protected:
    GLuint _program;
    GLuint _vertexShader, _geometryShader, _fragmentShader;

private:
    typedef std::map<std::string,UniformInfo> UniformInfoMap;
    typedef std::map<std::string,UniformInfoMap*> UniformBlockInfoMap;

private:
    UniformBlockInfoMap _uniformBlockData;

    friend class ShaderManager;
};

#endif
