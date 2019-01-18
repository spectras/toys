#ifndef SHADER_H_9D365E73
#define SHADER_H_9D365E73

#include <string>
#include <vector>
#include "gl/common.h"

namespace gl {

/****************************************************************************/

/// Object wrapper for OpenGL shaders
class Shader final
{
    using id_type = GLuint;                         ///< Internal type of Shader identifier
    static constexpr id_type invalid_id = 0;        ///< Sentinel value for empty shader
public:
    /// Kind of shader
    enum class type {
        Compute = GL_COMPUTE_SHADER,                ///< General purpose computations
        Vertex = GL_VERTEX_SHADER,                  ///< Vertex-level tranformation
        TessControl = GL_TESS_CONTROL_SHADER,       ///< Tesselation-controlling shader
        TessEvaluation = GL_TESS_EVALUATION_SHADER, ///< Tessalation-generating shader
        Geometry = GL_GEOMETRY_SHADER,              ///< Geometry-level transformation
        Fragment = GL_FRAGMENT_SHADER,              ///< Fragment-level transformation
    };
public:
    Shader() = default;                             ///< Create an invalid shader
    explicit Shader(id_type id) : m_id(id) {}       ///< Wrap OpenGL shader with given id
    Shader(const Shader &) = delete;
    Shader(Shader && rhs) noexcept { std::swap(m_id, rhs.m_id); }
    ~Shader()
        { clear(); }

    Shader & operator=(Shader && rhs)
    {
        clear();
        std::swap(m_id, rhs.m_id);
        return *this;
    }

    id_type     id() const noexcept                 ///< Get OpenGL shader identifier
        { return m_id; }

    void        clear()                             ///< Delete wrapped shader, if any.
    {
        if (m_id != invalid_id) {
            glDeleteShader(m_id);
            m_id = invalid_id;
        }
    }

    bool        hasError() const;                   ///< True if shader compilation failed
    std::string error() const;                      ///< Compilation logs

    /// Compile shader from glsl code in given buffer
    static Shader compile(type, const void * data, std::size_t len);
    /// Compile shader from glsl code in given buffer
    static Shader compile(type, const std::string & data);

private:
    id_type     m_id = invalid_id;                  ///< OpenGL shader identifier
};

/****************************************************************************/

/// Object wrapper for OpenGL program
class Program final
{
    using id_type = GLuint;                         ///< Internal type of Program identifier
    static constexpr id_type invalid_id = 0;        ///< Sentinel value for empty program
public:
    Program() = default;                            ///< Create an invalid program
    explicit Program(id_type id) : m_id(id) {}      ///< Wrap OpenGL program with given id
    Program(const Program &) = delete;
    Program(Program && rhs) noexcept { std::swap(m_id, rhs.m_id); }
    ~Program()
        { clear(); }

    Program & operator=(Program && rhs)
    {
        clear();
        std::swap(m_id, rhs.m_id);
        return *this;
    }

    id_type     id() const noexcept                 ///< Get OpenGL program identifier
        { return m_id; }

    void        clear()                             ///< Delete wrapped program, if any.
    {
        if (m_id != invalid_id) {
            glDeleteProgram(m_id);
            m_id = invalid_id;
        }
    }

    bool        hasError() const;                   ///< True if program linkage failed
    std::string error() const;                      ///< Linkage logs

    void        enable() const                      ///< Make the program active in rendering state
    {
        assert(m_id != invalid_id);
        glUseProgram(m_id);
    }

    /// Link program from a set of shaders
    static Program link(std::vector<Shader *>);
    template<typename... Ts> static Program link(Ts&... args) { return link({&args...}); }

private:
    id_type     m_id = invalid_id;                  ///< OpenGL program identifier
};

/****************************************************************************/

}

#endif
