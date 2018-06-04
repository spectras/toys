#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include "gl/common.h"

using gl::error;

const char * error::messageForGlError(GLenum code)
{
    switch(code) {
        case GL_NO_ERROR: return "no error";
        case GL_INVALID_ENUM: return "invalid enum";
        case GL_INVALID_VALUE: return "value out of range";
        case GL_INVALID_OPERATION: return "invalid operation";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "invalid framebuffer operation";
        case GL_OUT_OF_MEMORY: return "out of memory";
        case GL_STACK_UNDERFLOW: return "stack underflow";
        case GL_STACK_OVERFLOW: return "stack overflow";
        default: return "Unknown error";
    }
}
