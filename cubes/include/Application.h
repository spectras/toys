#ifndef APPLICATION_H_198BB0AA
#define APPLICATION_H_198BB0AA

#include <atomic>
#include <memory>
#include <string>
#include <GL/gl.h>
#include "gl/Buffer.h"
#include "gl/Shader.h"
#include "gl/Vertex.h"

struct SDL_Window;
struct SDL_KeyboardEvent;
struct SDL_WindowEvent;

namespace std {
    /// Tell the STL how to properly destroy an SDL_Window
    template <> struct default_delete<SDL_Window> { void operator()(SDL_Window *p) const; };
}

/****************************************************************************/

class Application final
{
public:
    Application(std::string name);
    ~Application();

    int         run();                                  ///< Main rendering and event loop

    void        init();                                 ///< Post-construction initialization
    void        update(unsigned);                       ///< Update world state
    void        render();                               ///< Render current world state to screen

    bool        processErrors(const char *);            ///< Handle rendering errors in other methods

    void        onKeyDown(const SDL_KeyboardEvent &);   ///< Called when the user presses a key
    void        onKeyUp(const SDL_KeyboardEvent &);     ///< Called when the user releases a key
    void        onQuitEvent();                          ///< Called when the window managers wants us to close
    void        onWindowEvent(const SDL_WindowEvent &); ///< Called on window manager notifications

    void        quit();                                 ///< Signal run() it should exit. Thread-safe.

private:
    /// Create an OpenGL-enabled window
    static std::unique_ptr<SDL_Window> createWindow(const std::string & name);

private:
    std::atomic<bool>           m_quit;                 ///< When set, run() will exit
    std::unique_ptr<SDL_Window> m_window;               ///< Main application window

    gl::Program                 m_program;              ///< Shader program used for rendering
    gl::VertexArray             m_array;                ///< Fully loaded cube vertex array
    gl::VertexBuffer            m_cube;                 ///< Geometry for a single cube
    GLint                       m_mvpLocation;          ///< Location of mvp uniform variable

    unsigned                    m_angle;                ///< Current rotation angle for cubes
};

/****************************************************************************/

#endif
