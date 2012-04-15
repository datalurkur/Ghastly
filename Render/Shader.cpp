#include <Base/Assertion.h>
#include <Render/Shader.h>
#include <Render/GLHelper.h>

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
    if(!compileStatus) {
        GLchar *buffer;
        GLsizei bufferLength, actualLength;
        
        bufferLength=512;
        buffer = (GLchar*)calloc(sizeof(GLchar), bufferLength);
        glGetShaderInfoLog(shader, bufferLength, &actualLength, buffer);
        Error("Shader failed to compile: " << buffer);
        free(buffer);
        
        ASSERT(0);   
    }

    return shader;
}

GLint Shader::getUniformLocation(const std::string &uniformName) {
    GLint location;

    ASSERT(_program);
    location = glGetUniformLocation(_program, uniformName.c_str());

    if(location == -1) {
        Error("Failed to locate uniform " << uniformName);
         ASSERT(0);
    }

    return location;
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

UniformBuffer* Shader::createUniformBuffer(const std::string &uniformBlockName) {
    ASSERT(_program);
    UniformBuffer *uBuffer = new UniformBuffer(_program, uniformBlockName);

    if(uBuffer->setup()) {
        return uBuffer;
    } else {
        delete uBuffer;
    }
    return 0;
}

void Shader::enable() {
    ASSERT(_program);
    glUseProgram(_program);
}

void Shader::disable() {
    glUseProgram(0);
}