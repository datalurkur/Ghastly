#include <Render/BufferState.h>

// Buffer State
// ============
BufferState::BufferState(unsigned int numElements, GLenum elementType, unsigned int elementSize, void *buffer, GLenum bufferType):
    _numElements(numElements), _elementType(elementType), _elementSize(elementSize), _bufferType(bufferType)
{
    unsigned int totalByteSize;

    switch(elementType) {
    case GL_FLOAT:
        totalByteSize = numElements * elementSize * sizeof(float);
        break;
    default:
        ASSERT(0);
        break;
    };

    _buffer = malloc(totalByteSize);
    ASSERT(_buffer);
    memcpy(_buffer, buffer, totalByteSize);
}

BufferState::~BufferState() {
    if(_buffer) { free(_buffer); }
}

void BufferState::preRender() {
    glEnableClientState(_bufferType);
}

void BufferState::postRender() {
    glDisableClientState(_bufferType);
}

// Vertex Buffer State
// ===================
void VertexBufferState::preRender() {
    BufferState::preRender();
    glVertexPointer(_elementSize, _elementType, 0, _buffer);
}

// Tex Coord Buffer State
// ======================
void TexCoordBufferState::preRender() {
    BufferState::preRender();
    glTexCoordPointer(_elementSize, _elementType, 0, _buffer);
}

// Color Buffer State
// ==================
void ColorBufferState::preRender() {
    BufferState::preRender();
    glColorPointer(_elementSize, _elementType, 0, _buffer);
}

// Normal Buffer State
// ===================
NormalBufferState::NormalBufferState(unsigned int numElements, GLenum elementType, void *buffer, GLenum bufferType):
    BufferState(numElements, elementType, 3, buffer, bufferType) {}

void NormalBufferState::preRender() {
    BufferState::preRender();
    glNormalPointer(_elementType, 0, _buffer);
}
