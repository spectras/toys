#define GL_GLEXT_PROTOTYPES
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <SDL.h>
#include "Application.h"

/****************************************************************************/

/// The main application object
static std::unique_ptr<Application> app;

/// Signal handler that tells `app` to quit as soon as possible
static void quit_handler(int) { app->quit(); }


int main(int, char *[])
{
    SDL_version version;
    SDL_GetVersion(&version);
    std::cerr <<"Running on SDL " <<int(version.major) <<'.'
                                  <<int(version.minor) <<'.'
                                  <<int(version.patch) <<std::endl;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        std::cerr <<SDL_GetError() <<std::endl;
    }
    std::atexit(SDL_Quit);
    SDL_GL_SetSwapInterval(1);

    // Initialize the application
    app = std::unique_ptr<Application>(new Application("cubes"));

    // Intercept signals to be able to quit gracefully
    std::signal(SIGINT, quit_handler);
    std::signal(SIGQUIT, quit_handler);
    std::signal(SIGTERM, quit_handler);
    std::signal(SIGHUP, SIG_IGN);

    // Run the main event loop
    auto retcode = app->run();

    // Disable signal handlers so they don't use app after it is deleted
    std::signal(SIGINT, SIG_IGN);
    std::signal(SIGQUIT, SIG_DFL);
    std::signal(SIGTERM, SIG_DFL);

    // Destroy the application
    app = nullptr;

    return retcode;
}
