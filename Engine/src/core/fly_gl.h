#ifndef FLY_GL_H
#define FLY_GL_H

#include <GL/glew.h>

#ifdef FLY_DEBUG_BUILD
#include <assert.h>
#include <stdio.h>

// do while trick, so that macro has a neccessary ';' at the end
#define GLCALL(x) do {                                                             \
    while (glGetError() != GL_NO_ERROR);                                           \
    x;                                                                             \
    while (GLenum error = glGetError()) {                                          \
        switch (error) {                                                           \
            case GL_INVALID_ENUM: {                                                \
                fprintf(stderr, "[FLY_ERROR]: GL_INVALID_ENUM\n");                 \
                assert(false);                                                     \
                break;                                                             \
            }                                                                      \
            case GL_INVALID_VALUE: {                                               \
                fprintf(stderr, "[FLY_ERROR]: GL_INVALID_VALUE\n");                \
                assert(false);                                                     \
                break;                                                             \
            }                                                                      \
            case GL_INVALID_OPERATION: {                                           \
                fprintf(stderr, "[FLY_ERROR]: GL_INVALID_OPERATION\n");            \
                assert(false);                                                     \
                break;                                                             \
            }                                                                      \
            case GL_INVALID_FRAMEBUFFER_OPERATION: {                               \
                fprintf(stderr, "[FLY_ERROR]: GL_INVALID_FRAMEBUFFER_OPERATION\n");\
                assert(false);                                                     \
                break;                                                             \
            }                                                                      \
            case GL_OUT_OF_MEMORY: {                                               \
                fprintf(stderr, "[FLY_ERROR]: GL_OUT_OF_MEMORY");                  \
                assert(false);                                                     \
                break;                                                             \
            }                                                                      \
            default: {                                                             \
                break;                                                             \
            }                                                                      \
        }                                                                          \
    }                                                                              \
} while(0)

#else
#define GLCALL(x) x
#endif
      
#endif /* End of FLY_GL_H */
