#include <cassert>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

#include "gl/Shader.h"

#include "gl/common.h"

using gl::Shader;
using gl::Program;


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

    assert(logSize >= 0);
    auto buffer = std::string(std::size_t(logSize) - 1, '\0');
    glGetShaderInfoLog(m_id, logSize - 1, nullptr, &buffer[0]);
    return buffer;
}

Shader Shader::compile(type t, const void * ptr, std::size_t size)
{
    auto id = glCreateShader(static_cast<GLenum>(t));
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

    assert(logSize >= 0);
    auto buffer = std::string(std::size_t(logSize) - 1, '\0');
    glGetProgramInfoLog(m_id, logSize - 1, nullptr, &buffer[0]);
    return buffer;
}

Program Program::link(std::vector<Shader *> shaders)
{
    auto id = glCreateProgram();
    if (id == invalid_id) { throw gl::error("glCreateProgram failed"); }

    for (auto & shader : shaders) { glAttachShader(id, shader->id()); }
    glLinkProgram(id);
    for (auto & shader : shaders) { glDetachShader(id, shader->id()); }

    return Program(id);
}
