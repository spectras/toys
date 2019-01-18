#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#include "gl/Vertex.h"

using gl::VertexArray;

constexpr GLuint VertexArray::invalid_id;
GLuint VertexArray::s_bound = VertexArray::invalid_id;

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_id);
    if (m_id == invalid_id) { throw error("glGenVertexArrays failed"); }
}
