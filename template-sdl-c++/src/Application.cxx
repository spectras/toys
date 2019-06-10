#include "Application.h"

#include <SDL.h>
#include <cmath>
#include <iostream>

static constexpr float PI = 3.14159265359f;

namespace app {

void SDLDeleter<SDL_Renderer>::operator()(SDL_Renderer * ptr) const { SDL_DestroyRenderer(ptr); }
void SDLDeleter<SDL_Window>::operator()(SDL_Window * ptr) const { SDL_DestroyWindow(ptr); }

/****************************************************************************/

static sdl_ptr<SDL_Window> createWindow(const char * name, int width, int height)
{
    auto window = sdl_ptr<SDL_Window>(SDL_CreateWindow(
        name,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height, SDL_WINDOW_SHOWN
    ));
    if (!window) { throw std::runtime_error(SDL_GetError()); }

    return window;
}

static sdl_ptr<SDL_Renderer> createRenderer(SDL_Window & window, Uint32 flags)
{
    auto renderer = sdl_ptr<SDL_Renderer>(SDL_CreateRenderer(&window, -1, flags));
    if (!renderer) { throw std::runtime_error(SDL_GetError()); }

    return renderer;
}

/****************************************************************************/

Application::Application(const char * name)
 : m_window(createWindow(name, 800, 600)),
   m_renderer(createRenderer(*m_window, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)),
   m_quit(false)
{}

Application::~Application() = default;


void Application::run()
{
    auto lastTicks = milliseconds(SDL_GetTicks());

    while (!m_quit) {
        // Handle any waiting event
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch (event.type) {
            case SDL_KEYDOWN:       onKeyDown(event.key); break;
            case SDL_KEYUP:         onKeyUp(event.key); break;
            case SDL_QUIT:          onQuitEvent(); break;
            case SDL_WINDOWEVENT:   onWindowEvent(event.window); break;
            }
        }

        // Keep track of elapsed time and feed it to update()
        auto ticks = milliseconds(SDL_GetTicks());
        update(ticks - lastTicks);
        lastTicks = ticks;

        if (m_visible) {
            render(m_renderer.get());
            SDL_RenderPresent(m_renderer.get());
        }
    }
}

// Update the state of the world - empty for still displays
void Application::update(milliseconds ms)
{
    m_angle += float(ms.count()) / 1000.0f;
    if (m_angle > 2 * PI) { m_angle -= 2 * PI; }
}

// Render a full view of current state of the world
void Application::render(SDL_Renderer * renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    const int xc = 400, yc = 300; // center position
    const float radius = 200.0f;
    const SDL_Point points[] = {
        {xc + int(radius * std::sin(m_angle + 0.0f*PI/3.0f)),
         yc + int(radius * std::cos(m_angle + 0.0f*PI/3.0f))},
        {xc + int(radius * std::sin(m_angle + 2.0f*PI/3.0f)),
         yc + int(radius * std::cos(m_angle + 2.0f*PI/3.0f))},
        {xc + int(radius * std::sin(m_angle + 4.0f*PI/3.0f)),
         yc + int(radius * std::cos(m_angle + 4.0f*PI/3.0f))},
        {xc + int(radius * std::sin(m_angle + 0.0f*PI/3.0f)),
         yc + int(radius * std::cos(m_angle + 0.0f*PI/3.0f))},
    };
    SDL_SetRenderDrawColor(renderer, 128, 192, 255, 0);
    SDL_RenderDrawLines(renderer, points, std::size(points));
}

/****************************************************************************/

void Application::onKeyDown(const SDL_KeyboardEvent & evt) noexcept
{
    switch (evt.keysym.sym) {
        case SDLK_ESCAPE: quit(); break;
    }
}

void Application::onKeyUp(const SDL_KeyboardEvent &) noexcept
{
    // nothing
}

void Application::onQuitEvent() noexcept
{
    quit();
}

void Application::onWindowEvent(const SDL_WindowEvent & evt) noexcept
{
    switch (evt.event) {
        case SDL_WINDOWEVENT_SHOWN: m_visible = true; break;
        case SDL_WINDOWEVENT_HIDDEN: m_visible = false; break;
    }
}

/****************************************************************************/

}
