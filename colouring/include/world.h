/** @file
 * Game world.
 *
 * Implements game rules for the colouring game. This is a one-player game, in
 * which the player has to fill a grid of coloured cells starting from the
 * top-left corner. At every turn, the player picks a color to be color-flooded
 * from that corner, making advances as the flooded area reachs other cells
 * of that colour.
 */
#ifndef WORLD_H
#define WORLD_H

#include <stdbool.h>

typedef unsigned char color_t;  /**< A color index in game world */
typedef struct world World;     /**< Opaque structure representing a game world */

/** World generator type.
 *
 * World generator is passed to world_create() to initialize the grid.
 *
 * @param[out] cells The grid to fill, containing `width`×`height` cells arranged in
 *             [row-major](https://en.wikipedia.org/wiki/Row-_and_column-major_order)
 *             order.
 * @param width Width of the grid in cells.
 * @param height Height of the grid in cells.
 * @param nb_colors Number of colors in the world.
 * @return `true` on success, `false` on failure. Returning `false` will cause
 *         world_create() to return `NULL`.
 * @post All cells have a color value within `[0, nb_colors[`.
 * @sa world_default_seeder(), world_random_seeder()
 */
typedef bool (*world_seeder_t)(color_t * cells, unsigned width, unsigned height,
                               unsigned nb_colors);

/****************************************************************************/
/** @name World Lifetime
 *  @{ */

/** Create game world
 * @param width Grid width in cells.
 * @param height Grid height in cells.
 * @param nb_colors The number of different colors in the world.
 * @param seeder The algorithm to use for world generation.
 * @return The created world, or `NULL` on error.
 */
World * world_create(unsigned width, unsigned height, unsigned nb_colors,
                     world_seeder_t seeder);

/** Destroy game world
 * @param [in] world The world to destroy.
 */
void world_destroy(World * world);

/** @} */
/****************************************************************************/
/** @name World Information
 *  @{ */

/** Get all dimensions of the game world
 * @param[in] world The world.
 * @param[out] width Pointer to the variable to store world width in cells into.
 *                   Can be `NULL`.
 * @param[out] height Pointer to the variable to store world height in cells into.
 *                   Can be `NULL`.
 * @param[out] nb_colors Pointer to the variable to store color number into.
 *                   Can be `NULL`.
 */
void world_get_dimensions(const World * world, unsigned * width, unsigned * height,
                          unsigned * nb_colors);

/** Get number of turned played so far
 * @param[in] world The world.
 * @return The number of turned played, starting from `0` for a freshly-created
 *         game.
 */
unsigned world_get_played_turns(const World * world);

/** Get color of a cell
 * @param[in] world The world.
 * @param x,y Coordinates of the cell, 0-based.
 * @return Current color of the cell, an integer in range `[0, nb_colors[`.
 * @note Passing out-of-bounds coordinates is a mistake.
 */
color_t world_get_cell(const World * world, unsigned x, unsigned y);

/** Test wheter win conditions are fulfilled
 * @param[in] world The world.
 * @return `true` if player has won the game, `false` otherwise.
 */
bool world_game_is_won(const World * world);

/** @}
 *  @name World Manipulation
 *  @{ */

/** Play a single turn
 * @param[in] world The world.
 * @param color The color to play. Must be withing range `[0, nb_colors[`
 *              otherwise result is undefined.
 * @post `world_get_played_turns(world)` increased by one.
 * @note Playing the same color twice is considered a valid move. Grid will
 *       not change but turn count will still be incremented.
 */
void world_play(World * world, color_t color);

/** @} */
/****************************************************************************/
/** @name World Generators
 *  @{ */

/** Default @ref world_seeder_t
 *
 * Generates a world semi-randomly to make the game more interesting. Compared
 * to world_random_seeder(), it has a much higher chance to yield contiguous
 * chunks with the same color.
 *
 * See documentation of @ref world_seeder_t for arguments.
 */
bool world_default_seeder(color_t *, unsigned width, unsigned height, unsigned nb_colors);

/** Purely random @ref world_seeder_t
 *
 * Generates a world randomly. The genuine feel, but maybe a bit boring.
 *
 * See documentation of @ref world_seeder_t for arguments.
 */
bool world_random_seeder(color_t *, unsigned width, unsigned height, unsigned nb_colors);

/** @} */

#if defined WORLD_INTERNALS || defined DOXYGEN
/**/
struct world {
    unsigned    width;              /**< Width of the world in cells */
    unsigned    height;             /**< Height of the world in cells */
    unsigned    nb_colors;          /**< Number of colors. All colors must be lower */

    unsigned    nb_played_turns;    /**< How many turns were played so far */

    color_t *   grid;               /**< Row-major array of cells */
};
#endif

#endif
