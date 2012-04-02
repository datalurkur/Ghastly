#ifndef GLHELPER_H
#define GLHELPER_H

#define CheckGLErrors() CheckGLErrorsFL(__FILE__, __LINE__)
extern void CheckGLErrorsFL(const char *file, int line);

#endif