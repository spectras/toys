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

/****************************************************************************/

/** GPU Buffer
 *
 * A buffer is a chunk of data accessible to the GPU.
 * Buffer types are incompatible, and each have a global binding point.
 * That is, as most one buffer of each type may be bound at any time, and binding
 * another buffer of the same type simply unbinds the previous one.
 *
 * When compiled in debug mode, all methods will assert this object really is
 * the currently bound buffer for the relevant target. Those checks are removed
 * if NDEBUG is defined.
 */
template <target Target> class Buffer final
{
    using id_type = GLuint;                     ///< Iternal type of buffer identifier
    static constexpr id_type invalid_id = 0;    ///< Sentinel value for invalid buffer object

public:
    /// Hints to driver for optimisation
    enum class usage {
        StaticDraw = GL_STATIC_DRAW,    ///< Data is intialized once and used to draw many times
        DynamicDraw = GL_DYNAMIC_DRAW,  ///< Data is updated often and used to draw many times
        StreamDraw = GL_STREAM_DRAW,    ///< Data is updated often and used to draw a few times
        StaticRead = GL_STATIC_READ,    ///< Data is intialized once and read from app many times
        DynamicRead = GL_DYNAMIC_READ,  ///< Data is updated often and read from app many times
        StreamRead = GL_STREAM_READ,    ///< Data is updated often and read from app a few times
        StaticCopy = GL_STATIC_COPY,    ///< Data is intialized once and copied to other buffer many times
        DynamicCopy = GL_DYNAMIC_COPY,  ///< Data is updated often and used to read from GL many times
        StreamCopy = GL_STREAM_COPY,    ///< Data is updated often and used to read from GL a few times
    };

public:
    Buffer() : m_id(invalid_id), m_size(0)
    {
        glGenBuffers(1, &m_id);
        if (m_id == invalid_id) { throw gl::error("glGenBuffers failed"); }
    }

    // A buffer cannot be copied
    Buffer(const Buffer &) = delete;
    Buffer & operator=(const Buffer & rhs) = delete;

    // A buffer supports move semantics
    Buffer(Buffer && rhs) noexcept : m_id(invalid_id), m_size(0)
        { swap(rhs); }
    Buffer & operator=(Buffer && rhs)
    {
        if (this != &rhs) {
            clear();
            swap(rhs);
        }
        return *this;
    }

    ~Buffer()
        { clear(); }

    /// Destroy underlying OpenGL buffer, making this object invalid
    void clear()
    {
        if (m_id == invalid_id) { return; }
        glDeleteBuffers(1, &m_id);
        m_id = invalid_id;
    }

    /// Get OpenGL buffer identifier
    id_type id() const noexcept
        { return m_id; }

    /// Get size of buffer in bytes
    std::size_t size() const noexcept
        { return m_size; }

    /// Discard data
    void setData(std::nullptr_t)
    {
        assert(s_bound == m_id);
        glBufferData(static_cast<GLenum>(Target), m_size, nullptr, GL_DYNAMIC_DRAW);
    }

    /// Discard data and resize buffer
    void setData(std::nullptr_t, std::size_t size)
    {
        assert(s_bound == m_id);
        m_size = size;
        glBufferData(static_cast<GLenum>(Target), size, nullptr, GL_DYNAMIC_DRAW);
    }

    /// Initialize a new data area from a raw buffer
    void setData(const void * ptr, std::size_t size, usage u)
    {
        assert(s_bound == m_id);
        m_size = size;
        glBufferData(static_cast<GLenum>(Target), size, ptr, static_cast<GLenum>(u));
    }

    /// Initialize a new data area from a container type
    template <typename T> void setData(const T & data, usage u)
    {
        setData(data.data(), data.size() * sizeof(typename T::value_type), u);
    }

    /// Update data from a raw buffer
    void setData(const void * ptr, std::size_t size, std::size_t offset)
    {
        assert(s_bound == m_id);
        assert(offset + size <= m_size);
        glBufferSubData(static_cast<GLenum>(Target), offset, size, ptr);
    }

    /// Update data from a container type
    template <typename T> void setData(const T & data, std::size_t offset)
    {
        setData(data.data(), data.size() * sizeof(typename T::value_type), offset);
    }

    /// Bind buffer, enabling the use of other methods
    void bind() const {
        if (s_bound == m_id) { return; }
        glBindBuffer(static_cast<GLenum>(Target), m_id);
        s_bound = m_id;
    }

    /// Unbind buffer, leaving no active bound buffer
    static void unbind() {
        if (s_bound == invalid_id) { return; }
        glBindBuffer(static_cast<GLenum>(Target), invalid_id);
        s_bound = invalid_id;
    }

    /// Get id of currently bound OpenGL buffer
    static id_type bound() noexcept { return s_bound; }

    /// Swap this buffer with another
    void swap(Buffer & rhs) noexcept
    {
        using std::swap;
        swap(m_id, rhs.m_id);
        swap(m_size, rhs.m_size);
    }
private:
    id_type     m_id;           ///< OpenGL buffer identifier
    std::size_t m_size;         ///< Size of buffer in bytes

    static id_type s_bound;     ///< Identifier of buffer that is currently bound
};

/// Swap two buffers, enabling ADL semantics
template <target Target> void swap(Buffer<Target> & lhs, Buffer<Target> & rhs) noexcept
{
    lhs.swap(rhs);
}

template <target Target> constexpr typename Buffer<Target>::id_type Buffer<Target>::invalid_id;
template <target Target> typename Buffer<Target>::id_type Buffer<Target>::s_bound = invalid_id;

/****************************************************************************/

// Convenient aliases for all targets
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
