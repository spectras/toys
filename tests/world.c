#include <check.h>
#include <stdint.h>
#define WORLD_INTERNALS
#include "world.h"
/****************************************************************************/

static bool dummy_seeder(color_t * grid, unsigned width, unsigned height, unsigned nb_colors)
{
    (void)nb_colors;
    for (unsigned i = 0; i < width * height; i += 1) { grid[i] = 0; }
    return true;
}


struct world_test_case {
    unsigned width, height, nb_colors;
    const uint8_t (*before)[];
    color_t  play;
    const uint8_t (*after)[];
};

static World * test_case_world(const struct world_test_case * data)
{
    World * world = world_create(data->width, data->height, data->nb_colors, dummy_seeder);
    if (world == NULL) { return NULL; }
    memcpy(world->grid, data->before, data->width * data->height * sizeof(world->grid[0]));
    return world;
}

static bool world_grid_eq(const World * world, const uint8_t (*grid)[])
{
    return memcmp(world->grid, grid,
                  world->width * world->height * sizeof(world->grid[0])) == 0;
}

static const struct world_test_case sample_worlds[] = {
    {   /* Basic test */
        6, 5, 2,
        &(const uint8_t[]){ 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0,
                            1, 1, 1, 1, 1, 1,
                            1, 1, 1, 1, 1, 1,
                            1, 1, 1, 1, 1, 1 },
        2,
        &(const uint8_t[]){ 2, 2, 2, 2, 2, 2,
                            2, 2, 2, 2, 2, 2,
                            1, 1, 1, 1, 1, 1,
                            1, 1, 1, 1, 1, 1,
                            1, 1, 1, 1, 1, 1 },
    },
    {   /* Complex shape with flood going upwards */
        6, 5, 2,
        &(const uint8_t[]){ 0, 0, 0, 0, 0, 0,
                            1, 1, 1, 1, 0, 1,
                            1, 0, 1, 1, 0, 1,
                            1, 0, 0, 0, 0, 0,
                            1, 1, 1, 1, 1, 0 },
        2,
        &(const uint8_t[]){ 2, 2, 2, 2, 2, 2,
                            1, 1, 1, 1, 2, 1,
                            1, 2, 1, 1, 2, 1,
                            1, 2, 2, 2, 2, 2,
                            1, 1, 1, 1, 1, 2},
    },
    {   /* Color blocking flood propagation */
        6, 5, 2,
        &(const uint8_t[]){ 0, 0, 0, 0, 0, 2,
                            1, 1, 1, 1, 1, 0,
                            1, 0, 1, 0, 0, 1,
                            1, 1, 1, 0, 0, 1,
                            1, 1, 0, 1, 1, 0 },
        2,
        &(const uint8_t[]){ 2, 2, 2, 2, 2, 2,
                            1, 1, 1, 1, 1, 0,
                            1, 0, 1, 0, 0, 1,
                            1, 1, 1, 0, 0, 1,
                            1, 1, 0, 1, 1, 0},
    }
};


/****************************************************************************/

START_TEST(test_world_init)
{
    const unsigned width = 5, height = 6, nb_colors = 8;
    unsigned w, h, n;
    unsigned x, y;
    bool ok = true;
    World * world = world_create(width, height, nb_colors, dummy_seeder);
    ck_assert_ptr_ne(world, NULL);

    world_get_dimensions(world, &w, &h, &n);
    ck_assert_uint_eq(w, width);
    ck_assert_uint_eq(h, height);
    ck_assert_uint_eq(n, nb_colors);
    ck_assert_uint_eq(world_get_played_turns(world), 0);

    for (y = 0; y < height; y += 1) {
        for (x = 0; x < width; x += 1) {
            if (world_get_cell(world, x, y) != 0) { ok = false; }
        }
    }
    ck_assert_msg(ok, "At least one cell was not properly initialized");

    world_destroy(world);
}
END_TEST

START_TEST(test_world_plays)
{
    for (unsigned i = 0; i < sizeof(sample_worlds) / sizeof(sample_worlds[0]); i += 1) {
        World * world = test_case_world(&sample_worlds[i]);
        ck_assert_ptr_ne(world, NULL);

        world_play(world, sample_worlds[i].play);
        ck_assert_msg(
            world_grid_eq(world, sample_worlds[i].after),
            "Play number %d did not yield expected result", i
        );
        world_destroy(world);
    }
}
END_TEST

START_TEST(test_world_win)
{
    const unsigned width = 5, height = 6, nb_colors = 8;
    World * world = world_create(width, height, nb_colors, dummy_seeder);
    ck_assert_ptr_ne(world, NULL);

    ck_assert(world_game_is_won(world));
    world_destroy(world);
}
END_TEST

/****************************************************************************/

Suite * build_world_suite()
{
    Suite * s = suite_create("world");
    TCase * tc = tcase_create("Core");
    tcase_add_test(tc, test_world_init);
    tcase_add_test(tc, test_world_plays);
    tcase_add_test(tc, test_world_win);

    suite_add_tcase(s, tc);
    return s;
}
