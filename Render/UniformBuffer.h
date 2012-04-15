#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

#include <Base/Base.h>

class UniformBuffer {
public:
    UniformBuffer(GLuint program);
    ~UniformBuffer();

    bool setup(const std::string &blockName);
    void teardown();

    void enable();
    void disable();

    void setParameter(const std::string &name, const void *data);
    
private:
    struct UniformInfo {
        GLuint index;
        GLint offset;
        GLint size;

        UniformInfo();
        UniformInfo(GLuint i, GLint o, GLint s);
        UniformInfo(const UniformInfo &other);
    };

private:
    void fetchUniformData();
    const UniformInfo& getUniformData(const std::string &name);

    void uploadPartialBufferData(unsigned int offset, unsigned int size, const void *data);
    void uploadBufferData(const void *data);

private:
    GLuint _program;

    GLint _blockIndex;
    GLint _blockSize;
    GLuint _bufferHandle;

    std::map<std::string,UniformInfo> _uniformData;
};

#endif
