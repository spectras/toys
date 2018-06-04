#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#include <SDL.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cassert>
#include <iostream>
#include <type_traits>
#include "gl/Shader.h"
#include "Application.h"
#include "resources.h"

/****************************************************************************/

/// Vertex format used for our cube
struct vertex {
    GLbyte   x, y, z;           ///< Integer coordinates, to be normalized to [-1, 1]
    glm::tvec3<GLubyte>  color; ///< R8G8B8
};

// Ensure the vertex has standard layout, enabling us to access members through offsets
static_assert(std::is_standard_layout<vertex>::value, "vertex must have standard layout");
// Ensure vertex data can be copied as raw bytes
static_assert(std::is_trivially_copyable<vertex>::value, "vertex must be trivially copyable");

static gl::VertexBuffer loadCubeData();

/****************************************************************************/

void std::default_delete<SDL_Window>::operator()(SDL_Window *p) const
{
    SDL_DestroyWindow(p);
}

/****************************************************************************/

Application::Application(std::string name)
 : m_quit(false),
   m_window(createWindow(name)),
   m_angle(0.0f)
{}

Application::~Application()
{}

int Application::run()
{
    init();

    auto lastTicks = SDL_GetTicks();
    do {
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

        auto ticks = SDL_GetTicks();
        update(ticks - lastTicks);
        render();
        SDL_GL_SwapWindow(m_window.get());

        processErrors("mainloop errors");
        lastTicks = ticks;
    } while(!m_quit);

    std::cerr <<"Exiting" <<std::endl;
    return 0;
}

void Application::init()
{
    // Load shaders
    auto vertexShader = gl::Shader::compile(gl::Shader::type::Vertex,
                                            shaders_vertex_glsl, shaders_vertex_glsl_len);
    if (vertexShader.hasError()) { throw std::runtime_error(vertexShader.error()); }
    auto fragmentShader = gl::Shader::compile(gl::Shader::type::Fragment,
                                              shaders_fragment_glsl, shaders_fragment_glsl_len);
    if (fragmentShader.hasError()) { throw std::runtime_error(fragmentShader.error()); }

    m_program = gl::Program::link(vertexShader, fragmentShader);
    if (m_program.hasError()) { throw std::runtime_error(m_program.error()); }

    m_mvpLocation = glGetUniformLocation(m_program.id(), "mvp");

    // Configure data array to read from cube data buffer
    m_cube = loadCubeData();
    m_array.bind();
    m_array.setVertexAttrib(0, m_cube, 3, gl::type::Byte, true,     // Vertex data is 3x 1 signed byte
                            sizeof(vertex), offsetof(vertex, x));
    m_array.enableVertexAttrib(0, true);                            // Shader will see it at pos 0
    m_array.setVertexAttrib(1, m_cube, 3, gl::type::UByte, true,    // Color data is 3x 1 unsigned byte
                            sizeof(vertex), offsetof(vertex, color));
    m_array.enableVertexAttrib(1, true);                            // Shader will see it at pos 1


    if (!processErrors("initialization errors")) {
        throw std::runtime_error("Application() failed");
    }
}

void Application::update(unsigned ms)
{
    m_angle += 200 * ms / 1000;
    if (m_angle >= 10000) { m_angle -= 10000; }
}

void Application::render()
{
    // Reset rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    auto projection = glm::perspective(glm::radians(45.0f), 4.0f/3.0f, 0.1f, 100.0f);
    m_program.enable();

    // Draw right cube
    auto model = glm::translate(glm::mat4(1), {2.0f, 0.0f, -8.0f});
    model = glm::rotate(model, 2.0f * glm::pi<float>() * float(m_angle) / 1000.0f, {0.0f, 1.0f, 0.0f});
    model = glm::rotate(model, 2.0f * glm::pi<float>() * float(m_angle) / 10000.0f, {0.0f, 0.0f, 1.0f});

    auto mvp = projection * model;
    glUniformMatrix4fv(m_mvpLocation, 1, GL_FALSE, &mvp[0][0]);
    m_array.draw(gl::primitive::Triangles, {0, 36});

    // Draw left cube
    model = glm::translate(glm::mat4(1), {-2.0f, 0.0f, -8.0f});
    model = glm::rotate(model, -2.0f * glm::pi<float>() * float(m_angle) / 1000.0f, {0.0f, 1.0f, 0.0f});
    model = glm::rotate(model, 2.0f * glm::pi<float>() * float(m_angle) / 10000.0f, {1.0f, 0.0f, 0.0f});

    mvp = projection * model;
    glUniformMatrix4fv(m_mvpLocation, 1, GL_FALSE, &mvp[0][0]);
    m_array.draw(gl::primitive::Triangles, {0, 36});
}

bool Application::processErrors(const char * ctx)
{
    GLenum err = glGetError();
    if (err == GL_NO_ERROR) { return true; }

    std::cerr <<ctx <<": \n";
    do {
        auto error = gl::error(err);
        std::cerr <<'\t' <<error.what() <<std::endl;
        err = glGetError();
    } while(err != GL_NO_ERROR);
    return false;
}

/****************************************************************************/

void Application::onKeyDown(const SDL_KeyboardEvent & evt)
{
    if (evt.keysym.sym == SDLK_ESCAPE) { quit(); }
}

void Application::onKeyUp(const SDL_KeyboardEvent &)
{
}

void Application::onQuitEvent()
{
    quit();
}

void Application::onWindowEvent(const SDL_WindowEvent &)
{
}

/****************************************************************************/

void Application::quit()
{
    m_quit = true;
}

std::unique_ptr<SDL_Window> Application::createWindow(const std::string & name)
{
    SDL_GL_ResetAttributes();
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    auto window = std::unique_ptr<SDL_Window>(SDL_CreateWindow(
        name.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        800, 600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN // | SDL_WINDOW_RESIZABLE
    ));
    if (!window) { throw std::runtime_error(SDL_GetError()); }

    SDL_GL_CreateContext(window.get());
    return window;
}

/****************************************************************************/

static constexpr std::array<vertex, 36> cubeVertice = {{
    // Left side
    { -127,-127,-127, { 0, 0, 255 }},
    { -127,-127, 127, { 255, 0, 255 }},
    { -127, 127, 127, { 255, 0, 0 }},
    { -127,-127,-127, { 0, 0, 255 }},
    { -127, 127, 127, { 255, 0, 0 }},
    { -127, 127,-127, { 255, 0, 255 }},
    // Back side
    { 127, 127,-127,  { 0, 255, 0 }},
    { -127,-127,-127, { 0, 0, 255 }},
    { -127, 127,-127, { 0, 255, 255 }},
    { 127, 127,-127,  { 0, 255, 0 }},
    { 127,-127,-127,  { 0, 255, 255 }},
    { -127,-127,-127, { 0, 0, 255 }},
    // Down side
    { 127,-127, 127,  { 255, 0, 0 }},
    { -127,-127,-127, { 0, 255, 0 }},
    { 127,-127,-127,  { 255, 255, 0 }},
    { 127,-127, 127,  { 255, 0, 0 }},
    { -127,-127, 127, { 255, 255, 0 }},
    { -127,-127,-127, { 0, 255, 0 }},
    // Front side
    { -127, 127, 127, { 192, 192, 0 }},
    { -127,-127, 127, { 0, 255, 0 }},
    { 127,-127, 127,  { 0, 192, 192 }},
    { 127, 127, 127,  { 0, 255, 0 }},
    { -127, 127, 127, { 192, 192, 0 }},
    { 127,-127, 127,  { 0, 192, 192 }},
    // Right side
    { 127, 127, 127,  { 192, 0, 192 }},
    { 127,-127,-127,  { 192, 192, 0 }},
    { 127, 127,-127,  { 255, 0, 0 }},
    { 127,-127,-127,  { 192, 192, 0 }},
    { 127, 127, 127,  { 192, 0, 192 }},
    { 127,-127, 127,  { 255, 0, 0 }},
    // Top side
    { 127, 127, 127,  { 0, 192, 192 }},
    { 127, 127,-127,  { 192, 0, 192 }},
    { -127, 127,-127, { 0, 0, 255 }},
    { 127, 127, 127,  { 0, 192, 192 }},
    { -127, 127,-127, { 0, 0, 255 }},
    { -127, 127, 127, { 192, 0, 192 }}
}};

static gl::VertexBuffer loadCubeData()
{
    gl::VertexBuffer buffer;
    buffer.bind();
    buffer.setData(cubeVertice, gl::VertexBuffer::usage::StaticDraw);
    return buffer;
}
