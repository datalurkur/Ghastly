#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

#include <Base/Base.h>

class UniformBuffer {
public:
    UniformBuffer();
    ~UniformBuffer();

    bool setup(GLuint program, const std::string &blockName);
    void teardown();

    void enable();
    void disable();

    void setParameter(const std::string &name, const void *data);

private:
    void fetchUniformData(GLint program);

    GLuint getUniformIndex(const std::string &name);
    GLint getUniformOffset(GLuint index);
    GLint getUniformSize(GLuint index);

    void uploadPartialBufferData(unsigned int offset, unsigned int size, const void *data);
    void uploadBufferData(const void *data);

private:
    GLint _blockIndex;
    GLint _blockSize;
    GLuint _bufferHandle;

    std::map<std::string,GLuint> _uniformIndices;
    std::map<GLuint,GLint> _uniformOffsets;
    std::map<GLuint,GLint> _uniformSizes;
};

#endif
