#ifndef BUFFERSTATE_H
#define BUFFERSTATE_H

#include <Render/GenericRenderState.h>
#include <Base/Base.h>
#include <SDL/SDL_opengl.h>

class BufferState: public GenericRenderState {
public:
    BufferState(unsigned int numElements, GLenum elementType, unsigned int elementSize, void *buffer, GLenum bufferType);
    ~BufferState();

    void preRender();
    void postRender();

protected:
    unsigned int _numElements;
    GLenum _elementType;
    unsigned int _elementSize;
    void *_buffer;

private:
    GLenum _bufferType;
};

class VertexBufferState: public BufferState {
public:
    VertexBufferState(unsigned int numElements, GLenum elementType, unsigned int elementSize, void *buffer);
    void preRender();
};

class TexCoordBufferState: public BufferState {
public:
    TexCoordBufferState(unsigned int numElements, GLenum elementType, unsigned int elementSize, void *buffer);
    void preRender();
};

class ColorBufferState: public BufferState {
public:
    ColorBufferState(unsigned int numElements, GLenum elementType, unsigned int elementSize, void *buffer);
    void preRender();
};

class NormalBufferState: public BufferState {
public:
    NormalBufferState(unsigned int numElements, GLenum elementType, void *buffer);
    void preRender();
};

#endif
