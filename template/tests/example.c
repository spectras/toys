#include <check.h>
#include "example.h"

START_TEST(test_example_works)
{
    ck_assert_str_eq(example_hello(), "hello");
}
END_TEST

/****************************************************************************/

Suite * build_example_suite()
{
    Suite * s = suite_create("example");
    TCase * tc = tcase_create("Core");

    /* Add tests here: */
    tcase_add_test(tc, test_example_works);

    suite_add_tcase(s, tc);
    return s;
}
