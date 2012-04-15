#ifndef BUFFERSTATE_H
#define BUFFERSTATE_H

#include <Render/GenericRenderState.h>
#include <Render/Shader.h>
#include <Base/Base.h>
#include <SDL/SDL_opengl.h>

class BufferState: public GenericRenderState {
public:
    BufferState(GLuint numElements, GLenum elementType, GLuint elementSize, void *buffer, GLenum bufferType);
    ~BufferState();

    void preRender();
    void postRender();

private:
    void setupAttribState(GLuint attribBindPoint);

protected:
    GLuint _numElements;
    GLenum _elementType;
    GLuint _elementSize;
    void *_buffer;

private:
    GLenum _bufferType;

    GLuint _attribBindPoint;
    bool _useAttribs;

public:
    static bool AreAttribsEnabled();

    // Static factory methods
    // These are used primarily as a point of branching from various buffer types into shader-enabled attribs or the default <buffer>Pointer behavior (deprecated)
    static BufferState *VertexBuffer(GLuint numElements, GLenum elementType, GLuint elementSize, void *buffer, Shader *shader);
    static BufferState *NormalBuffer(GLuint numElements, GLenum elementType, void *buffer, Shader *shader);
    static BufferState *TexCoordBuffer(GLuint numElements, GLenum elementType, GLuint elementSize, void *buffer, Shader *shader);
    static BufferState *ColorBuffer(GLuint numElements, GLenum elementType, GLuint elementSize, void *buffer, Shader *shader);
};

#endif
