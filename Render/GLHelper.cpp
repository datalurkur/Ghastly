#include <Base/Assertion.h>

void CheckGLErrorsFL(const char* file, int line) {
    GLenum errCode;
    const GLubyte *errStr;
    if ((errCode = glGetError()) != GL_NO_ERROR) {
        errStr = gluErrorString(errCode);
        Error("OpenGL error at " << file << ":" << line << " - " << (char*)errStr);
        ASSERT(false);
    }
}
