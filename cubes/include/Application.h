#ifndef APPLICATION_H_198BB0AA
#define APPLICATION_H_198BB0AA

#include <atomic>
#include <chrono>
#include <memory>
#include <string>
#include <GL/gl.h>
#include "gl/Buffer.h"
#include "gl/Shader.h"
#include "gl/Vertex.h"

struct SDL_Window;
struct SDL_KeyboardEvent;
struct SDL_WindowEvent;

// unique_ptr variants that properly destroy SDL resources
template <typename T> struct SDLDeleter {};
template<> struct SDLDeleter<SDL_Window> { void operator()(SDL_Window *) const; };
template <typename T> using sdl_ptr = std::unique_ptr<T, SDLDeleter<T>>;

/****************************************************************************/

class Application final
{
    using milliseconds = std::chrono::duration<unsigned, std::milli>;
public:
    Application() = delete;
    explicit Application(std::string name);
    ~Application();

    int     run();                                  ///< Main rendering and event loop
    void    quit();                                 ///< Signal run() it should exit. Thread-safe.

private:
    void    init();                                 ///< Post-construction initialization
    void    update(milliseconds);                   ///< Update world state
    void    render();                               ///< Render current world state to screen

    bool    processErrors(const char *);            ///< Handle rendering errors in other methods

    void    onKeyDown(const SDL_KeyboardEvent &);   ///< Called when the user presses a key
    void    onKeyUp(const SDL_KeyboardEvent &);     ///< Called when the user releases a key
    void    onQuitEvent();                          ///< Called when the window managers wants us to close
    void    onWindowEvent(const SDL_WindowEvent &); ///< Called on window manager notifications

private:
    /// Create an OpenGL-enabled window
    static sdl_ptr<SDL_Window> createWindow(const std::string & name);

private:
    std::atomic<bool>   m_quit;                 ///< When set, run() will exit
    sdl_ptr<SDL_Window> m_window;               ///< Main application window

    gl::Program         m_program;              ///< Shader program used for rendering
    gl::VertexArray     m_array;                ///< Fully loaded cube vertex array
    gl::VertexBuffer    m_cube;                 ///< Geometry for a single cube
    GLint               m_mvpLocation = 0;      ///< Location of mvp uniform variable

    unsigned            m_angle = 0;            ///< Current rotation angle for cubes
    bool                m_visible = false;      ///< Whether window is currently visible
};

/****************************************************************************/

#endif
