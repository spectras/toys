#ifndef BUFFER_H_64C3AC75
#define BUFFER_H_64C3AC75

#include <cassert>
#include <utility>
#include "gl/common.h"

namespace gl {

/****************************************************************************/

/// Possible buffer types
enum class target {
    Array = GL_ARRAY_BUFFER,                            ///< Array of vertices
    ElementArray = GL_ELEMENT_ARRAY_BUFFER,             ///< Array of indices into vertices
    PixelPack = GL_PIXEL_PACK_BUFFER,                   ///< Pixel read buffer
    PixelUnpack = GL_PIXEL_UNPACK_BUFFER,               ///< Texture data source
    CopyRead = GL_COPY_READ_BUFFER,                     ///< Source of raw data copy
    CopyWrite = GL_COPY_WRITE_BUFFER,                   ///< Target of raw data copy
    TransformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER,   ///< Feedback data from transformations
    Texture = GL_TEXTURE_BUFFER,                        ///< Pixel data for texturing
    Uniform = GL_UNIFORM_BUFFER                         ///< Uniform data for shader use
};


/** GPU Buffer
 *
 * A buffer is a chunk of data accessible to the GPU.
 * Buffer types are incompatible, and each have a global binding point.
 * That is, as most one buffer of each type may be bound at any time, and binding
 * another buffer of the same type simply unbinds the previous one.
 */
template <target Target> class Buffer final
{
    typedef GLuint id_type;                 ///< Iternal type of buffer identifier
    static constexpr id_type invalid_id = 0;    ///< Sentinel value for invalid buffer object
public:

    /// Hints to driver for optimisation
    enum class usage {
        StaticDraw = GL_STATIC_DRAW,        ///< The data will be intialized once and read many times
        DynamicDraw = GL_DYNAMIC_DRAW,      ///< The data will be updated every now and then
        StreamDraw = GL_STREAM_DRAW,        ///< The data will be updated at every use
        StaticRead = GL_STATIC_READ,
        DynamicRead = GL_DYNAMIC_READ,
        StreamRead = GL_STREAM_READ,
        StaticCopy = GL_STATIC_COPY,
        DynamicCopy = GL_DYNAMIC_COPY,
        StreamCopy = GL_STREAM_COPY,
    };

public:
    Buffer()
     : m_id(invalid_id), m_size(0)
    {
        glGenBuffers(1, &m_id);
        if (m_id == invalid_id) { throw gl::error("glGenBuffers failed"); }
    }

    Buffer(const Buffer &) = delete;
    Buffer(Buffer && other) noexcept
        : m_id(invalid_id), m_size(0)
        { std::swap(m_id, other.m_id); std::swap(m_size, other.m_size); }
    Buffer &    operator=(const Buffer &) = delete;

    Buffer & operator=(Buffer && other)
    {
        assert(this != &other);
        if (m_id != invalid_id) { glDeleteBuffers(1, &m_id); }
        m_id = other.m_id;
        m_size = other.m_size;
        other.m_id = invalid_id;
        return *this;
    }

    ~Buffer()
    {
        if (m_id != invalid_id) { glDeleteBuffers(1, &m_id); }
    }

    id_type     id() const noexcept { return m_id; }
    std::size_t size() const noexcept
        { return m_size; }                  ///< Buffer length, in bytes

    /// Discard data
    inline void setData(std::nullptr_t)
    {
        assert(s_bound == m_id);
        glBufferData(static_cast<GLenum>(Target), m_size, nullptr, GL_DYNAMIC_DRAW);
    }

    /// Discard data and resize buffer
    inline void setData(std::nullptr_t, std::size_t size)
    {
        assert(s_bound == m_id);
        m_size = size;
        glBufferData(static_cast<GLenum>(Target), size, nullptr, GL_DYNAMIC_DRAW);
    }

    /// Initialize a new data area from a raw buffer
    inline void setData(const void * ptr, std::size_t size, usage u)
    {
        assert(s_bound == m_id);
        m_size = size;
        glBufferData(static_cast<GLenum>(Target), size, ptr, static_cast<GLenum>(u));
    }

    /// Initialize a new data area from a container type
    template <typename T> inline void setData(const T & data, usage u)
    {
        setData(data.data(), data.size() * sizeof(typename T::value_type), u);
    }

    /// Update data from a raw buffer
    inline void setData(const void * ptr, std::size_t size, std::size_t offset)
    {
        assert(s_bound == m_id);
        assert(offset + size <= m_size);
        glBufferSubData(static_cast<GLenum>(Target), offset, size, ptr);
    }

    /// Update data from a container type
    template <typename T> inline void setData(const T & data, std::size_t offset)
    {
        setData(data.data(), data.size() * sizeof(typename T::value_type), offset);
    }

    /// Bind buffer, enabling the use of other methods
    inline void bind() const {
        if (s_bound == m_id) { return; }
        glBindBuffer(static_cast<GLenum>(Target), m_id);
        s_bound = m_id;
    }

    /// Unbind buffer, leaving no active bound buffer
    inline static void unbind() {
        if (s_bound == invalid_id) { return; }
        glBindBuffer(static_cast<GLenum>(Target), invalid_id);
        s_bound = invalid_id;
    }

    /// Get id of currently bound OpenGL buffer
    inline static id_type bound() noexcept { return s_bound; }
private:
    id_type     m_id;           ///< OpenGL buffer identifier
    std::size_t m_size;         ///< Size of buffer in bytes

    static id_type s_bound;     ///< Identifier of buffer that is currently bound
};


template <target Target> constexpr typename Buffer<Target>::id_type Buffer<Target>::invalid_id;
template <target Target> typename Buffer<Target>::id_type Buffer<Target>::s_bound = invalid_id;

/****************************************************************************/

typedef Buffer<target::Array> VertexBuffer;
typedef Buffer<target::ElementArray> ElementBuffer;
typedef Buffer<target::PixelPack> PixelPackBuffer;
typedef Buffer<target::PixelUnpack> PixelUnpackBuffer;
typedef Buffer<target::TransformFeedback> TransformFeedbackBuffer;
typedef Buffer<target::Texture> TextureBuffer;
typedef Buffer<target::Uniform> UniformBuffer;

/****************************************************************************/

}

#endif
