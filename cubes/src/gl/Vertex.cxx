#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#include "gl/Vertex.h"

using gl::VertexArray;
using gl::primitive;

constexpr GLuint VertexArray::invalid_id;
GLuint VertexArray::s_bound = VertexArray::invalid_id;

VertexArray::VertexArray() : m_id(invalid_id)
{
    glGenVertexArrays(1, &m_id);
    if (m_id == invalid_id) { throw error("glGenVertexArrays failed"); }
}

VertexArray & VertexArray::operator=(VertexArray && rhs)
{
    clear();
    std::swap(m_id, rhs.m_id);
    return *this;
}

VertexArray::~VertexArray()
{
    clear();
}

void VertexArray::clear()
{
    if (m_id == invalid_id) { return; }
    glDeleteVertexArrays(1, &m_id);
    m_id = invalid_id;
}
