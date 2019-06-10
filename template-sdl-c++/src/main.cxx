#include <SDL.h>
#include <csignal>
#include <iostream>
#include "Application.h"

using app::Application;

static std::unique_ptr<Application> application;
static void quitHandler(int) { application->quit(); }


int main()
{
    // Initialize SDL
    SDL_version version;
    SDL_GetVersion(&version);
    std::cerr <<"Initializing SDL " <<int(version.major) <<'.'
                                    <<int(version.minor) <<'.'
                                    <<int(version.patch) <<"..." <<std::endl;
    if (SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        std::cerr <<"SDL initialization failed: " <<SDL_GetError() <<'\n';
        return 1;
    }
    std::atexit(SDL_Quit);

    // Run application
    application = std::make_unique<Application>("template-sdl-c++");

    // Intercept signals to be able to quit gracefully
    std::signal(SIGINT, quitHandler);
    std::signal(SIGQUIT, quitHandler);
    std::signal(SIGTERM, quitHandler);
    std::signal(SIGHUP, SIG_IGN);

    application->run();

    // Disable signal handlers so they don't use application after it is deleted
    std::signal(SIGINT, SIG_IGN);
    std::signal(SIGQUIT, SIG_DFL);
    std::signal(SIGTERM, SIG_DFL);

    application = nullptr;

    return 0;
}
