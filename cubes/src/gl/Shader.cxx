#include <cassert>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

#include "gl/Shader.h"

#include "gl/common.h"

using gl::Shader;
using gl::Program;

Shader::Shader(id_type id)
 : m_id(id)
{}

Shader & Shader::operator=(Shader && rhs)
{
    clear();
    std::swap(m_id, rhs.m_id);
    return *this;
}

Shader::~Shader()
{
    clear();
}

void Shader::clear()
{
    if (m_id != invalid_id) {
        glDeleteShader(m_id);
        m_id = invalid_id;
    }
}

bool Shader::hasError() const
{
    assert(m_id != invalid_id);
    GLint status;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &status);
    return status != GL_TRUE;
}

std::string Shader::error() const
{
    assert(m_id != invalid_id);
    GLint logSize;
    glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &logSize);
    if (logSize == 0) { return {}; }

    auto buffer = std::string(logSize - 1, '\0');
    glGetShaderInfoLog(m_id, logSize - 1, nullptr, &buffer[0]);
    return buffer;
}

Shader Shader::compile(type t, const void * ptr, std::size_t size)
{
    id_type id = glCreateShader(static_cast<GLenum>(t));
    if (id == invalid_id) { throw gl::error("glCreateShader failed"); }

    const char * strings[1] = { reinterpret_cast<const char *>(ptr) };
    const GLint lengths[1] = { GLint(size) };
    glShaderSource(id, 1, strings, lengths);
    glCompileShader(id);

    return Shader(id);
}

Shader Shader::compile(type t, const std::string & code)
{
    return compile(t, code.data(), code.size());
}

/****************************************************************************/

Program::Program(id_type id)
 : m_id(id)
{}

Program & Program::operator=(Program && rhs)
{
    clear();
    std::swap(m_id, rhs.m_id);
    return *this;
}

Program::~Program()
{
    clear();
}

void Program::clear()
{
    if (m_id != invalid_id) {
        glDeleteProgram(m_id);
        m_id = invalid_id;
    }
}

bool Program::hasError() const
{
    assert(m_id != invalid_id);
    GLint status;
    glGetProgramiv(m_id, GL_LINK_STATUS, &status);
    return status != GL_TRUE;
}

std::string Program::error() const
{
    assert(m_id != invalid_id);
    GLint logSize;
    glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &logSize);
    if (logSize == 0) { return {}; }

    auto buffer = std::string(logSize - 1, '\0');
    glGetProgramInfoLog(m_id, logSize - 1, nullptr, &buffer[0]);
    return buffer;
}

void Program::enable() const
{
    assert(m_id != invalid_id);
    glUseProgram(m_id);
}

Program Program::link(std::vector<Shader *> shaders)
{
    id_type id = glCreateProgram();
    if (id == invalid_id) { throw gl::error("glCreateProgram failed"); }

    for (auto & shader : shaders) { glAttachShader(id, shader->id()); }
    glLinkProgram(id);
    for (auto & shader : shaders) { glDetachShader(id, shader->id()); }

    return Program(id);
}
