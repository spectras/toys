/** @file
 * Program entry point.
 *
 * Parses options from command line, initializes libraries and instanciates
 * game world / interface.
 */
/** @page LICENSE
 *  @verbinclude LICENSE
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <SDL.h>
#include "config.h"
#include "colouring.h"
#include "world.h"

/** Program options, parsed from command line */
struct options {
    unsigned    verbosity;  /**< Logging verbosity */
    unsigned    width;      /**< Game width in cells */
    unsigned    height;     /**< Game height in cells */
    unsigned    nb_colors;  /**< Number of colors in game world */
    unsigned    seed;       /**< Game world seed (can be forced to get same world again) */
    unsigned    turns;      /**< Maximum number of turns to flood the whole world */
};

/** Parse options from the command line
 * @param argc Number of tokens on the command line
 * @param[in,out] argv Command line tokens
 * @param[out] opts The option structure to fill from arguments
 * @return `true` if command line could be parsed correctly, `false`
 *         otherwise.
 * @post If parse_options() returns `true`, then options in `opts` are
 *       fully initialized, either from command line values or known defaults.
 */
bool parse_options(int argc, char * argv[], struct options * opts)
{
    struct timespec now;
    char opt;
    opts->verbosity  = 0;
    opts->width      = 20;
    opts->height     = 15;
    opts->nb_colors  = 6;
    clock_gettime(CLOCK_REALTIME, &now);
    opts->seed       = now.tv_sec * 1024 + now.tv_nsec/1024;
    opts->turns      = 0;

    while ((opt = getopt(argc, argv, "hn:s:t:v")) != -1) {
        switch (opt) {
        case 'n':
            opts->nb_colors = strtoul(optarg, NULL, 10);
            if (opts->nb_colors < 3 || opts->nb_colors > COLOURING_MAX_COLORS) {
                fprintf(stderr, "Color number must be between 3 and %d\n", COLOURING_MAX_COLORS);
                return false;
            }
            break;
        case 's':
            opts->seed = strtoul(optarg, NULL, 10);
            break;
        case 't':
            opts->turns = strtoul(optarg, NULL, 10);
            break;
        case 'v':
            opts->verbosity += 1;
            break;
        case 'h':
        default:
            fprintf(stderr, "Usage: %s [-n colors] [-s seed] [-t turns] "
                            "[-v] [width x height]\n",
                    argv[0]);
            return false;
        }
    }

    if (optind < argc) {
        if (sscanf(argv[optind], "%ux%u", &opts->width, &opts->height) != 2) {
            fprintf(stderr, "Dimensions must be in format <width>x<height>\n");
            return false;
        }
        if (opts->width < 3 || opts->width > COLOURING_MAX_WIDTH) {
            fprintf(stderr, "Width must be between 3 and %d\n", COLOURING_MAX_WIDTH);
            return false;
        }
        if (opts->height < 3 || opts->height > COLOURING_MAX_HEIGHT) {
            fprintf(stderr, "Width must be between 3 and %d\n", COLOURING_MAX_HEIGHT);
            return false;
        }
    }
    /* If no turn count was given, make up a reasonably challenging one */
    if (opts->turns == 0) {
        opts->turns = opts->nb_colors * 3;
    }

    return true;
}

/** Program entry point
 * @param argc Number of tokens on the command line.
 * @param[in] argv Table of tokens from the command line.
 * @return `EXIT_SUCCESS` on success, a non-zero value otherwise.
 */
int main(int argc, char * argv[])
{
    struct options options;
    SDL_version version;
    World * world;
    Colouring * app;
    int exit_code;

    if (!parse_options(argc, argv, &options)) { return 1; }

    /* Initialize SDL */
    SDL_GetVersion(&version);
    fprintf(stderr, "Initializing SDL %d.%d.%d...\n",
            version.major, version.minor, version.patch);

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 2;
    }
    atexit(SDL_Quit);

    /* Create game world */
    fprintf(stderr, "Generating %dx%d grid with seed %u...\n",
            options.width, options.height, options.seed);
    srand(options.seed);
    world = world_create(options.width, options.height,
                         options.nb_colors, world_default_seeder);
    if (world == NULL) { exit_code = 3; goto err_shutdown_sdl; }

    /* Run application */
    app = colouring_create(world, options.turns);
    if (app == NULL) { exit_code = 4; goto err_destroy_world; }
    exit_code = colouring_exec(app);
    colouring_destroy(app);

    /* Cleanup */
err_destroy_world:
    world_destroy(world);
err_shutdown_sdl:
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    return exit_code;
}
