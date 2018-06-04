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
public:
    typedef GLuint id_type;                 ///< Iternal type of buffer identifier
    static constexpr id_type invalid_id = 0;    ///< Sentinel value for invalid buffer object

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

    /// Buffer access permissions for mappings
    enum class access {
        Read = GL_READ_ONLY,
        Write = GL_WRITE_ONLY,
        ReadWrite = GL_READ_WRITE
    };

public:
    /** User-space mapped part of a buffer
     *
     * Mapping a buffer (or part of it) makes its data accessible to the program through
     * a regular pointer.
     */
    template <typename T> class mapping final
    {
    public:
        typedef T               value_type;
        typedef T &             reference;
        typedef const T &       const_reference;
        typedef T *             iterator;
        typedef const T *       const_iterator;
        typedef std::ptrdiff_t  difference_type;
        typedef std::size_t     size_type;

    public:
        mapping() noexcept
            : m_buffer(nullptr), m_addr(nullptr), m_size(0) {}
        mapping(Buffer * buffer, T * addr, size_type size) noexcept
            : m_buffer(buffer), m_addr(addr), m_size(size) {}
        mapping(const mapping<T> &) = delete;
        mapping(mapping<T> && rhs) noexcept
            : m_buffer(rhs.m_buffer), m_addr(nullptr), m_size(rhs.m_size)
            { std::swap(m_addr, rhs.m_addr); }
        mapping & operator=(const mapping <T> &) = delete;
        mapping & operator=(mapping<T> && rhs)
        {
            clear();
            std::swap(m_buffer, rhs.m_buffer);
            std::swap(m_addr, rhs.m_addr);
            std::swap(m_size, rhs.m_size);
        }
        ~mapping()
            { clear();  }

        operator        bool() const noexcept { return bool(m_addr); }
        void            clear()
        {
            if (!m_addr) { return; }
            glUnmapBuffer(static_cast<GLenum>(Target));
            m_addr = nullptr;
        }
        Buffer &        buffer() const noexcept { return *m_buffer; }

        iterator        begin() { return m_addr; }
        const_iterator  begin() const { return m_addr; }
        const_iterator  cbegin() const { return m_addr; }
        iterator        end() { return m_addr + m_size; }
        const_iterator  end() const { return m_addr + m_size; }
        const_iterator  cend() const { return m_addr + m_size; }
        reference       operator[](size_type i) { return m_addr[i]; }
        const_reference operator[](size_type i) const { return m_addr[i]; }

        T *             data() noexcept { return m_addr; }
        const T *       data() const noexcept { return m_addr; }
        bool            empty() const noexcept { return bool(m_addr); }
        size_type       size() const noexcept { return m_size; }

        void flush()
        {
            glFlushMappedBufferRange(m_buffer->target(), 0, m_size);
        }

        void flush(size_type offset, size_type size)
        {
            assert(offset + size <= m_size);
            glFlushMappedBufferRange(m_buffer->target(), offset * sizeof(T), size * sizeof(T));
        }

    private:
        Buffer *    m_buffer;   ///< buffer object this mapping grants access to
        T *         m_addr;     ///< address of first element of the mapping
        size_type   m_size;     ///< total number of elements made accessible
    };

public:
    Buffer();
    Buffer(const Buffer &) = delete;
    Buffer(Buffer && other) noexcept
        : m_id(invalid_id), m_size(0)
        { std::swap(m_id, other.m_id); std::swap(m_size, other.m_size); }
    Buffer &    operator=(const Buffer &) = delete;
    Buffer &    operator=(Buffer &&);
                ~Buffer();

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

    /// Map the whole buffer into process' address space
    template <typename T> mapping<T> map(access a)
    {
        assert(s_bound == m_id);
        auto ptr = reinterpret_cast<T*>(glMapBuffer(static_cast<GLenum>(Target), static_cast<GLenum>(a)));
        if (!ptr) { throw gl::error(glGetError()); }
        return mapping<T>(this, ptr, m_size / sizeof(T));
    }

    /// Map a range of the buffer into process' address space
    template <typename T> mapping<T> map(std::size_t pos, std::size_t size, access a)
    {
        assert(s_bound == m_id);
        assert((pos + size) * sizeof(T) <= m_size);
        auto ptr = reinterpret_cast<T*>(glMapBufferRange(static_cast<GLenum>(Target),
                                                         pos * sizeof(T), size * sizeof(T),
                                                         static_cast<GLenum>(a)));
        if (!ptr) { throw gl::error(glGetError()); }
        return mapping<T>(this, ptr, size);
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

template <target Target> Buffer<Target>::Buffer()
 : m_id(invalid_id), m_size(0)
{
    glGenBuffers(1, &m_id);
    if (m_id == invalid_id) { throw gl::error("glGenBuffers failed"); }
}

template <target Target> Buffer<Target> & Buffer<Target>::operator=(Buffer<Target> && other)
{
    assert(this != &other);
    if (m_id != invalid_id) { glDeleteBuffers(1, &m_id); }
    m_id = other.m_id;
    m_size = other.m_size;
    other.m_id = invalid_id;
    return *this;
}

template <target Target> Buffer<Target>::~Buffer()
{
    if (m_id != invalid_id) { glDeleteBuffers(1, &m_id); }
}

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
