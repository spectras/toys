/** @file
 * @copydoc colouring.h
 */
#include <SDL.h>
#include <stdlib.h>
#include "colouring.h"
#include "image.h"
#include "resources.h"
#include "world.h"

/****************************************************************************/

/** Minimum number of pixels between windows border and first square in
 * each direction
 */
static const int border_width = 5;

/** Table of colors.
 * @note This table must have \ref COLOURING_MAX_COLORS elements.
 */
static const SDL_Color colors[] = {
    {0xff, 0x00, 0x00, 0xff}, {0x00, 0xff, 0x00, 0xff}, {0x00, 0x00, 0xff, 0xff},
    {0xff, 0xff, 0x00, 0xff}, {0x00, 0xff, 0xff, 0xff}, {0xff, 0x00, 0xff, 0xff},
    {0x7f, 0x00, 0x00, 0xff}, {0x00, 0x7f, 0x00, 0xff}, {0x00, 0x00, 0x7f, 0xff},
    {0x7f, 0x7f, 0x00, 0xff}, {0x00, 0x7f, 0x7f, 0xff}, {0x7f, 0x00, 0x7f, 0xff},
    {0xff, 0x7f, 0x00, 0xff}, {0xff, 0x00, 0x7f, 0xff}, {0x7f, 0xff, 0x00, 0xff}
};

/****************************************************************************/

struct colouring {
    /** @name Resources
     *  @{ */
    World *         world;              /**< Game world *unowned* */
    unsigned        turns;              /**< Maximum number of turns */
    SDL_Window *    window;             /**< Application window */
    SDL_Renderer *  renderer;           /**< SDL renderer drawing on @ref window */
    SDL_Texture *   background;         /**< Background for @ref window */
    int             background_w;
    int             background_h;
    /** @} */

    /** @name Geometry
     *  @{ */
    unsigned        width;              /**< Cached width of the world in cells */
    unsigned        height;             /**< Cached height of the world in cells */
    unsigned        square_size;        /**< Single square dimension in pixels */
    int             hoffset;            /**< Grid horizontal offset in pixels */
    int             voffset;            /**< Grid vertical offset in pixels */
    /** @} */

    /** @name Event handling
     *  @{ */
    bool            mouse_down;         /**< Whether mouse_x and mouse_y are valid */
    unsigned        mouse_x;            /**< X-coordinate of cell the mouse button went
                                             down on */
    unsigned        mouse_y;            /**< Y-coordinate of cell the mouse button went
                                             down on */
    /** @} */
};

/****************************************************************************/
/** @name Application lifetime
 *  @{ */

Colouring * colouring_create(World * world, unsigned turns)
{
    SDL_Surface * surface;
    Colouring * app = malloc(sizeof(*app));
    if (app == NULL) { return NULL; }

    app->world = world;
    app->turns = turns;
    world_get_dimensions(world, &app->width, &app->height, NULL);
    app->mouse_down = false;

    app->window = SDL_CreateWindow("Colouring",
                                   SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   20 * app->width, 20 * app->height,
                                   SDL_WINDOW_RESIZABLE);
    if (app->window == NULL) { goto err_free_app; }
    app->renderer = SDL_CreateRenderer(app->window, -1, 0);
    if (app->renderer == NULL) { goto err_free_window; }

    surface = image_load_buffer(resources_background_png, resources_background_png_len);
    if (surface != NULL) {
        app->background = SDL_CreateTextureFromSurface(app->renderer, surface);
        app->background_w = surface->w;
        app->background_h = surface->h;
        SDL_FreeSurface(surface);
    }

    surface = image_load_buffer(resources_icon_png, resources_icon_png_len);
    if (surface != NULL) {
        SDL_SetWindowIcon(app->window, surface);
        SDL_FreeSurface(surface);
    }

    return app;

err_free_window:
    SDL_DestroyWindow(app->window);
err_free_app:
    free(app);
    return NULL;
}

void colouring_destroy(Colouring * app)
{
    SDL_DestroyTexture(app->background);
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
    free(app);
}

/** @} */
/****************************************************************************/
/** @name Rendering and Geometry
 *  @{ */

/** Recompute game representation geometry after a window resize
 * @param[in,out] app The interface
 */
static void recompute_geometry(Colouring * app)
{
    int win_width, win_height;
    int rect_width, rect_height;
    SDL_GetRendererOutputSize(app->renderer, &win_width, &win_height);
    rect_width = (win_width - 2 * border_width) / app->width;
    rect_height = (win_height - 2 * border_width) / app->height;
    app->square_size = rect_width < rect_height ? rect_width : rect_height;
    app->hoffset = (win_width - app->square_size * app->width) / 2;
    app->voffset = (win_height - app->square_size * app->height) / 2;
}

/** Render game world to window
 * @param[in] app The interface
 */
static void render(const Colouring * app)
{
    int win_x, win_y, win_width, win_height;
    unsigned x, y;

    /* Render background - texture if it was loaded, solid otherwise */
    if (app->background != NULL) {
        SDL_GetRendererOutputSize(app->renderer, &win_width, &win_height);
        for (win_y = 0; win_y < win_height; win_y += app->background_h) {
            for (win_x = 0; win_x < win_width; win_x += app->background_w) {
                SDL_RenderCopy(app->renderer, app->background, NULL,
                               &(SDL_Rect){win_x, win_y, app->background_w, app->background_h});
            }
        }
    } else {
        SDL_SetRenderDrawColor(app->renderer, 196, 178, 143, 255);
        SDL_RenderClear(app->renderer);
    }

    /* Render game tiles */
    for (y = 0; y < app->height; y += 1) {
        for (x = 0; x < app->width; x += 1) {
            const SDL_Color * color = &colors[world_get_cell(app->world, x, y)];
            SDL_SetRenderDrawColor(app->renderer, color->r, color->g, color->b, 255);
            SDL_RenderFillRect(app->renderer, &(SDL_Rect){
                app->hoffset + x * app->square_size,
                app->voffset + y * app->square_size,
                app->square_size - 1,
                app->square_size - 1
            });
        }
    }

    SDL_RenderPresent(app->renderer);
}

/** Compute world grid coordinates from window coordinates
 * @param[in] app The interface
 * @param[in] pos Window-relative coordinates to reverse.
 * @param[out] out_x,out_y Variables in which to store coordinates of grid cell.
 * @return `true` if the position is inside a cell, `false` otherwise. In
 *         the latter case, `out_x` and `out_y` are left unmodified.
 */
static bool reverse_coordinates(const Colouring * app, const SDL_Point * pos,
                                unsigned * out_x, unsigned * out_y)
{
    unsigned x, y;
    if (pos->x < app->hoffset || pos->y < app->voffset) { return false; }
    x = (pos->x - app->hoffset) / app->square_size;
    y = (pos->y - app->voffset) / app->square_size;
    if (x >= app->width || y >= app->height) { return false; }
    *out_x = x; *out_y = y;
    return true;
}

/** @} */
/****************************************************************************/
/** @name Event handling
 *  @{ */

/** Act upon user request to play a turn
 * @param [in,out] app The interface
 * @param color The color the user wants to play.
 */
static void do_play_turn(Colouring * app, color_t color)
{
    world_play(app->world, color);
    render(app);
    if (world_game_is_won(app->world)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game ended",
                                    "You won. Well played!",
                                    app->window);
        SDL_PushEvent(&(SDL_Event){SDL_QUIT});
    } else if (world_get_played_turns(app->world) >= app->turns) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game ended",
                                    "You lost. Better luck next time!",
                                    app->window);
        SDL_PushEvent(&(SDL_Event){SDL_QUIT});
    } else {
        char title[32];
        sprintf(title, "Colouring - turn %d",
                world_get_played_turns(app->world) + 1);
        SDL_SetWindowTitle(app->window, title);
    }
}

/** Handle mouse motion events
 *
 * Invoked everytime a mouse cursor moves inside the window
 * @param[in,out] app The interface
 * @param[in] event The [structure](https://wiki.libsdl.org/SDL_MouseMotionEvent)
 *                  describing mouse motion.
 */
static void on_mouse_motion(Colouring * app, const SDL_MouseMotionEvent * event)
{
    (void)app, (void)event;
    return;
}

/** Handle mouse down events
 *
 * Invoked everytime a mouse button is pressed while the cursor is inside the window.
 * @param[in,out] app The interface
 * @param[in] event The [structure](https://wiki.libsdl.org/SDL_MouseButtonEvent)
 *                  describing mouse motion.
 */
static void on_mouse_down(Colouring * app, const SDL_MouseButtonEvent * event)
{
    if (reverse_coordinates(app, &(SDL_Point){event->x, event->y},
                            &app->mouse_x, &app->mouse_y)) {
        app->mouse_down = true;
    } else {
        app->mouse_down = false;
    }
}

/** Handle mouse up events
 *
 * Invoked everytime a mouse button is released while the cursor is inside the window.
 * @param[in,out] app The interface
 * @param[in] event The [structure](https://wiki.libsdl.org/SDL_MouseButtonEvent)
 *                  describing mouse motion.
 */
static void on_mouse_up(Colouring * app, const SDL_MouseButtonEvent * event)
{
    unsigned x, y;
    if (!app->mouse_down) { return; }
    if (reverse_coordinates(app, &(SDL_Point){event->x, event->y}, &x, &y)) {
        if (x == app->mouse_x && y == app->mouse_y) {
            do_play_turn(app, world_get_cell(app->world, x, y));
        }
    }
    app->mouse_down = false;
}

/** Handle window manager events
 *
 * Invoked whe the window manager notifies the window of some state change
 * @param[in,out] app The interface
 * @param[in] event The [structure](https://wiki.libsdl.org/SDL_WindowEvent)
 *                  describing the window manager notification.
 */
static void on_window_event(Colouring * app, const SDL_WindowEvent * event)
{
    switch(event->event) {
    case SDL_WINDOWEVENT_SHOWN:
    case SDL_WINDOWEVENT_SIZE_CHANGED:
        recompute_geometry(app);
        /* fall-through */
    case SDL_WINDOWEVENT_EXPOSED:
        render(app);
        break;
    }
}

/** @} */
/****************************************************************************/
/** @name Main event loop
 *  @{ */

int colouring_exec(Colouring * app)
{
    bool quit = false;
    while (!quit) {
        SDL_Event event;
        if (!SDL_WaitEvent(&event)) {
            fprintf(stderr, "SDL_WaitEvent failed: %s\n", SDL_GetError());
            return 3;
        }
        switch (event.type) {
        case SDL_MOUSEMOTION:       on_mouse_motion(app, &event.motion); break;
        case SDL_MOUSEBUTTONDOWN:   on_mouse_down(app, &event.button); break;
        case SDL_MOUSEBUTTONUP:     on_mouse_up(app, &event.button); break;
        case SDL_WINDOWEVENT:       on_window_event(app, &event.window); break;
        case SDL_QUIT:
            quit = true;
            break;
        }
    }
    return EXIT_SUCCESS;
}

/** @} */
