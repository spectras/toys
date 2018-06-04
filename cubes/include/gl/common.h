#ifndef GL_COMMON_H_ECA71469
#define GL_COMMON_H_ECA71469

#include <stdexcept>

namespace gl {

class error : public std::runtime_error
{
public:
    /// Construct an error from an OpenGL error code
    explicit error(GLenum code) : std::runtime_error(messageForGlError(code)) {}

    /// Construct an error from a string description
    explicit error(const std::string & what) : std::runtime_error(what) {}
protected:
    static const char * messageForGlError(GLenum);
};


enum class primitive {
    Points = GL_POINTS,
    LineStrip = GL_LINE_STRIP,
    LineLoop = GL_LINE_LOOP,
    Lines = GL_LINES,
    LinesAdjacency = GL_LINES_ADJACENCY,
    TriangleStrip = GL_TRIANGLE_STRIP,
    TriangleFan = GL_TRIANGLE_FAN,
    Triangles = GL_TRIANGLES,
    TriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY
};

enum class type {
    Byte = GL_BYTE,
    UByte = GL_UNSIGNED_BYTE,
    Short = GL_SHORT,
    UShort = GL_UNSIGNED_SHORT,
    Int = GL_INT,
    UInt = GL_UNSIGNED_INT,
    Float = GL_FLOAT,
    HalfFloat = GL_HALF_FLOAT,
    Double = GL_DOUBLE,
    Fixed = GL_FIXED,
    Int2101010 = GL_INT_2_10_10_10_REV,
    UInt2101010 = GL_UNSIGNED_INT_2_10_10_10_REV,
    UInt101111 = GL_UNSIGNED_INT_10F_11F_11F_REV
};

}

#endif
