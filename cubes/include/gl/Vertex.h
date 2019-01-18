#ifndef VERTEX_H_19E3BDC5
#define VERTEX_H_19E3BDC5

#include <utility>
#include <vector>
#include "gl/common.h"
#include "gl/Buffer.h"

namespace gl {

/****************************************************************************/

/** GPU-based array of drawing data
 *
 * Ties together all non-uniform data sources into a single object that is fed to
 * the active shader program, producing output.
 */
class VertexArray final
{
public:
    using id_type = GLuint;                     ///< Internal type of vertex array identifier
    static constexpr id_type invalid_id = 0;    ///< Sentinel value for invalid vertex object
    using bounds = std::pair<GLsizei, GLsizei>;

public:
    VertexArray();
    VertexArray(const VertexArray &) = delete;
    VertexArray(VertexArray && other) noexcept
        : m_id(invalid_id) { std::swap(m_id, other.m_id); }
    ~VertexArray()
        { clear(); }

    VertexArray & operator=(VertexArray && rhs)
    {
        clear();
        std::swap(m_id, rhs.m_id);
        return *this;
    }

    void clear()
    {
        if (m_id != invalid_id) {
            glDeleteVertexArrays(1, &m_id);
            m_id = invalid_id;
        }
    }

    void enableVertexAttrib(GLuint idx, bool val)
    {
        assert(s_bound == m_id);
#ifndef NDEBUG
        assert(m_attribArrays.at(idx) != VertexBuffer::invalid_id);
#endif
        if (val) {
            glEnableVertexAttribArray(idx);
        } else {
            glDisableVertexAttribArray(idx);
        }
    }

    /// Define data for a vertex attribute
    void setVertexAttrib(GLuint idx, VertexBuffer & buf,
                         unsigned n, gl::type type, bool normalize,
                         GLsizei stride, GLsizei offset)
    {
        assert(s_bound == m_id);
#ifndef NDEBUG
        if (m_attribArrays.size() <= idx) {
            m_attribArrays.resize(idx + 1, VertexBuffer::invalid_id);
        }
        m_attribArrays[idx] = buf.id();
#endif
        buf.bind();
        glVertexAttribPointer(idx, GLint(n), static_cast<GLenum>(type), normalize,
                              stride, reinterpret_cast<void*>(offset));
    }

    /// Define data for a vertex attribute, using integers
    void setVertexAttribI(GLuint idx, VertexBuffer & buf, unsigned n, gl::type type,
                          GLsizei stride, GLsizei offset)
    {
        assert(s_bound == m_id);
#ifndef NDEBUG
        if (m_attribArrays.size() <= idx) {
            m_attribArrays.resize(idx + 1, VertexBuffer::invalid_id);
        }
        m_attribArrays[idx] = buf.id();
#endif
        buf.bind();
        glVertexAttribIPointer(idx, GLint(n), static_cast<GLenum>(type),
                               stride, reinterpret_cast<void*>(offset));
    }

    /// Define data for a vertex attribute, using double precision floats
    void setVertexAttribL(GLuint idx, VertexBuffer & buf, unsigned n, gl::type type,
                          GLsizei stride, GLsizei offset)
    {
        assert(s_bound == m_id);
#ifndef NDEBUG
        if (m_attribArrays.size() <= idx) {
            m_attribArrays.resize(idx + 1, VertexBuffer::invalid_id);
        }
        m_attribArrays[idx] = buf.id();
#endif
        buf.bind();
        glVertexAttribLPointer(idx, GLint(n), static_cast<GLenum>(type),
                               stride, reinterpret_cast<void*>(offset));
    }

    // Drawing - single

    /// Draw simple primitives from the vertex array, using flat vertex bounds
    void draw(primitive prim, bounds bnd) const
    {
        assert(s_bound == m_id);
        glDrawArrays(static_cast<GLenum>(prim), bnd.first, bnd.second);
    }

    /// Draw indexed primitives from the vertex array, using flat index bounds
    void drawIndices(primitive prim, gl::type type, bounds bnd) const
    {
        assert(s_bound == m_id);
        glDrawElements(static_cast<GLenum>(prim), bnd.second, static_cast<GLenum>(type),
                       reinterpret_cast<void*>(bnd.first));
    }

    /// Draw indexed primitives from the vertex array using index bounds with offset
    void drawIndices(primitive prim, gl::type type, bounds bnd, GLint base) const
    {
        assert(s_bound == m_id);
        glDrawElementsBaseVertex(static_cast<GLenum>(prim), bnd.second, static_cast<GLenum>(type),
                                 reinterpret_cast<void*>(bnd.first), base);
    }

    // Drawing - multiple

    /// Draw multiple primitives from the vertex array, using an array of flat vertex bounds
    void draw(primitive prim, const std::vector<GLint> & positions,
                              const std::vector<GLsizei> & counts) const
    {
        assert(s_bound == m_id);
        assert(positions.size() == counts.size());
        glMultiDrawArrays(static_cast<GLenum>(prim), positions.data(), counts.data(),
                          GLsizei(counts.size()));
    }

    /// Draw multiple indexed primitives from the vertex array, using an array of index bounds
    void drawIndices(primitive prim, gl::type type,
                     const std::vector<std::uintptr_t> & positions,
                     const std::vector<GLsizei> & counts) const
    {
        assert(s_bound == m_id);
        assert(positions.size() == counts.size());
        glMultiDrawElements(static_cast<GLenum>(prim), counts.data(), static_cast<GLenum>(type),
                            reinterpret_cast<void * const *>(positions.data()), GLsizei(counts.size()));
    }

    /// Draw multiple indexed primitives from the vertex array using an array of index bounds with offsets
    void drawIndices(primitive prim, gl::type type,
                     const std::vector<std::uintptr_t> & positions,
                     const std::vector<GLsizei> & counts,
                     const std::vector<GLint> & bases) const
    {
        assert(s_bound == m_id);
        assert(positions.size() == counts.size());
        assert(positions.size() == bases.size());
        glMultiDrawElementsBaseVertex(static_cast<GLenum>(prim), counts.data(),
                                      static_cast<GLenum>(type),
                                      reinterpret_cast<void * const *>(positions.data()),
                                      GLsizei(counts.size()), bases.data());
    }

    // Drawing - instanced

    /// Draw multiple instances of primitives from the vertex array, using flat vertex bounds
    void drawInstanced(primitive prim, bounds bnd, GLsizei instances) const
    {
        assert(s_bound == m_id);
        glDrawArraysInstanced(static_cast<GLenum>(prim), bnd.first, bnd.second, instances);
    }

    /// Draw multiple instances of primitives from the vertex array, using flat index bounds
    void drawIndicesInstanced(primitive prim, gl::type type,
                              bounds bnd, GLsizei instances) const
    {
        assert(s_bound == m_id);
        glDrawElementsInstanced(static_cast<GLenum>(prim), bnd.second, static_cast<GLenum>(type),
                                reinterpret_cast<void*>(bnd.first), instances);
    }

    /// Draw multiple instances of primitives from the vertex array, using index bounds with offset
    void drawIndicesInstanced(primitive prim, gl::type type,
                              bounds bnd, GLint base, GLsizei instances) const
    {
        assert(s_bound == m_id);
        glDrawElementsInstancedBaseVertex(static_cast<GLenum>(prim), bnd.second,
                                          static_cast<GLenum>(type),
                                          reinterpret_cast<void*>(bnd.first),
                                          base, instances);
    }

    /// Bind vertex array, enabling the use of other methods
    void bind() const
    {
        if (m_id == s_bound) { return; }
        glBindVertexArray(m_id);
        s_bound = m_id;
    }

    /// Unbind vertex array, leaving no active bound array
    static void unbind()
    {
        if (s_bound == invalid_id) { return; }
        glBindVertexArray(invalid_id);
        s_bound = invalid_id;
    }

private:
    id_type                 m_id = invalid_id;  ///< OpenGL vertex array identifier
#ifndef NDEBUG
    std::vector<id_type>    m_attribArrays;
#endif

    static id_type          s_bound;            ///< Identifier of buffer that is currently bound
};

/****************************************************************************/

}

#endif
