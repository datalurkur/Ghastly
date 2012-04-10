#include <Base/Assertion.h>
#include <Render/Shader.h>

Shader::Shader():
    _program(0), _vertexShader(0), _geometryShader(0), _fragmentShader(0)
{
}

Shader::~Shader() {
    teardown();
}

GLuint Shader::compile(const char *shaderProgramData, GLenum type) {
    GLuint shader;
    GLint compileStatus;

    shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderProgramData, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    ASSERT(compileStatus);

    return shader;
}

void Shader::setup(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader) {
    GLint linkStatus;

    if(!_program) {
        _program = glCreateProgram();
    }

    if(vertexShader) {
        _vertexShader = vertexShader;
        glAttachShader(_program, _vertexShader);
    }

    if(geometryShader) {
        _geometryShader = geometryShader;
        glAttachShader(_program, _geometryShader);
    }

    if(fragmentShader) {
        _fragmentShader = fragmentShader;
        glAttachShader(_program, _fragmentShader);
    }

    glLinkProgram(_program);
    glGetProgramiv(_program, GL_LINK_STATUS, &linkStatus);
    ASSERT(linkStatus);
}

void Shader::teardown() {
    if(_vertexShader) {
        glDetachShader(_program, _vertexShader);
        glDeleteShader(_vertexShader);
        _vertexShader = 0;
    }

    if(_geometryShader) {
        glDetachShader(_program, _geometryShader);
        glDeleteShader(_geometryShader);
        _geometryShader = 0;
    }

    if(_fragmentShader) {
        glDetachShader(_program, _fragmentShader);
        glDeleteShader(_fragmentShader);
        _fragmentShader = 0;
    }

    if(_program) {
        glDeleteProgram(_program);
        _program = 0;
    }
}

void Shader::enable() {
    ASSERT(_program);
    glUseProgram(_program);
}

void Shader::disable() {
    glUseProgram(0);
}

GLint Shader::getAttributeLocation(const std::string &attributeName) {
    return glGetAttribLocation(_program, attributeName.c_str());
}

GLint Shader::getUniformLocation(const std::string &uniformName) {
    return glGetUniformLocation(_program, uniformName.c_str());
}
