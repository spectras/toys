#ifndef APPLICATION_H
#define APPLICATION_H

#include <atomic>
#include <chrono>
#include <memory>
#include <string>

// Forward declare structs used in header to avoid including SDL.h
struct SDL_Renderer;
struct SDL_Window;
struct SDL_KeyboardEvent;
struct SDL_WindowEvent;

namespace app {

/****************************************************************************/

// unique_ptr variants that properly destroy SDL resources
template <typename T> struct SDLDeleter {};
template<> struct SDLDeleter<SDL_Renderer> { void operator()(SDL_Renderer *) const; };
template<> struct SDLDeleter<SDL_Window> { void operator()(SDL_Window *) const; };

template <typename T> using sdl_ptr = std::unique_ptr<T, SDLDeleter<T>>;

/****************************************************************************/

class Application final
{
    using milliseconds = std::chrono::duration<unsigned, std::milli>;
public:
    explicit    Application(const char * name);
                ~Application();

    void    run();
    void    quit() noexcept { m_quit = true; }

private:
    void    update(milliseconds);
    void    render(SDL_Renderer *);

    void    onKeyDown(const SDL_KeyboardEvent &) noexcept;
    void    onKeyUp(const SDL_KeyboardEvent &) noexcept;
    void    onQuitEvent() noexcept;
    void    onWindowEvent(const SDL_WindowEvent &) noexcept;

private:
    sdl_ptr<SDL_Window>     m_window;           // Main application window
    sdl_ptr<SDL_Renderer>   m_renderer;         // Main renderer

    std::atomic<bool>       m_quit;
    bool                    m_visible = false;  // Whether window is currently visible
    float                   m_angle = 0.0f;     // Current angle of the triangle
};

/****************************************************************************/

} // namespace main

#endif
