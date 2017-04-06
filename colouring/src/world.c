/** @file
 * @copydoc world.h
 */
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "stack.h"
#include "utils.h"
#define WORLD_INTERNALS
#include "world.h"

static void world_flood(World *, unsigned start_x, unsigned start_y, color_t color);

/****************************************************************************/

World * world_create(unsigned width, unsigned height, unsigned nb_colors,
                     world_seeder_t seeder)
{
    World * world = malloc(sizeof(World));
    if (world == NULL) { return NULL; }

    world->width = width;
    world->height = height;
    world->nb_colors = nb_colors;
    world->nb_played_turns = 0;
    world->grid = malloc(width * height * sizeof(world->grid[0]));
    if (world->grid == NULL || !seeder(world->grid, width, height, nb_colors)) {
        free(world);
        return NULL;
    }
    return world;
}

void world_destroy(World * world)
{
    free(world->grid);
    free(world);
}

void world_get_dimensions(const World * world, unsigned * width, unsigned * height,
                          unsigned * nb_colors)
{
    if (width != NULL) { *width = world->width; }
    if (height != NULL) { *height = world->height; }
    if (nb_colors != NULL) { *nb_colors = world->nb_colors; }
}

unsigned world_get_played_turns(const World * world) { return world->nb_played_turns; }

color_t world_get_cell(const World * world, unsigned x, unsigned y)
{
    return world->grid[y * world->width + x];
}

bool world_game_is_won(const World * world)
{
    color_t ref_color = world->grid[0];
    unsigned nb_cells = world->width * world->height;

    for (unsigned i = 0; i < nb_cells; i += 1) {
        if (world->grid[i] != ref_color) { return false; }
    }
    return true;
}

void world_play(World * world, color_t color)
{
    world_flood(world, 0, 0, color);
    world->nb_played_turns += 1;
}

/****************************************************************************/

/** Grid coordinates
 */
typedef struct {
    unsigned    x, y;
} Point;

static void world_flood(World * world, unsigned start_x, unsigned start_y, color_t color)
{
    assert(start_x < world->width);
    assert(start_y < world->height);

    color_t (*grid)[world->width] = (color_t(*)[world->width])world->grid;
    color_t target = grid[start_y][start_x];

    if (target == color) { return; }

    Stack * todo = stack_create(sizeof(Point));

    grid[start_y][start_x] = color;
    stack_push(todo, &(Point){start_x, start_y});

    while (stack_size(todo) > 0) {
        Point point;
        stack_pop(todo, &point);

        if (point.x > 0 && grid[point.y][point.x - 1] == target) {
            grid[point.y][point.x - 1] = color;
            stack_push(todo, &(Point){point.x - 1, point.y});
        }
        if (point.x < world->width - 1 && grid[point.y][point.x + 1] == target) {
            grid[point.y][point.x + 1] = color;
            stack_push(todo, &(Point){point.x + 1, point.y});
        }
        if (point.y > 0 && grid[point.y - 1][point.x] == target) {
            grid[point.y - 1][point.x] = color;
            stack_push(todo, &(Point){point.x, point.y - 1});
        }
        if (point.y < world->height - 1 && grid[point.y + 1][point.x] == target) {
            grid[point.y + 1][point.x] = color;
            stack_push(todo, &(Point){point.x, point.y + 1});
        }
    }

    stack_destroy(todo);
}

/****************************************************************************/

bool world_default_seeder(color_t * cells, unsigned width, unsigned height,
                          unsigned nb_colors)
{
    color_t (*grid)[width] = (color_t(*)[width])cells;
    unsigned x, y;
    for (y = 0; y < height; y += 1) {
        for (x = 0; x < width; x += 1) {
            int what = rand();
            if (what <= 1*(RAND_MAX/5) && x > 0) {
                grid[y][x] = grid[y][x-1];
            } else if (what > 1*(RAND_MAX/5) && what < 2*(RAND_MAX/5) && y > 0) {
                grid[y][x] = grid[y-1][x];
            } else {
                grid[y][x] = rand_interval(0, nb_colors - 1);
            }
        }
    }
    return true;
}

bool world_random_seeder(color_t * grid, unsigned width, unsigned height,
                         unsigned nb_colors)
{
    unsigned nb_cells = width * height;
    for (unsigned i = 0; i < nb_cells; i += 1) {
        grid[i] = rand_interval(0, nb_colors - 1);
    }
    return true;
}
