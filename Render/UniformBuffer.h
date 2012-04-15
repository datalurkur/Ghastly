#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

#include <Base/Base.h>
#include <Base/IndexPool.h>

class Shader;

class UniformBuffer {
// STATIC
// This section allows UniformBuffer to keep track of how many UBOs exist for each uniform block globally
private:
    static GLuint GetBindPoint(const std::string &blockName);
    static void ReleaseBindPoint(const std::string &blockName, GLuint bindPoint);

private:
    static IndexPool BlockIndices;

// NON STATIC
public:
    UniformBuffer(Shader *shader, const std::string &blockName);
    ~UniformBuffer();

    bool setup();
    void teardown();

    void enable();
    void disable();

    void setParameter(const std::string &name, const void *data);

private:
    //void fetchUniformData();
    //const UniformInfo& getUniformData(const std::string &name);

    void uploadPartialBufferData(unsigned int offset, unsigned int size, const void *data);
    void uploadBufferData(const void *data);

private:
    Shader *_shader;

    std::string _blockName;
    GLuint _bindPoint;

    GLint _blockIndex;
    GLint _blockSize;
    GLuint _bufferHandle;

    //std::map<std::string,UniformInfo> _uniformData;
};

#endif
