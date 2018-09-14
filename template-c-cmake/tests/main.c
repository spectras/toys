/* Main entry point for running tests.
 * Add test suites below.
 */
#include <stdlib.h>
#include <check.h>
#include "config.h"

/* Add the function declaration here: */
Suite * build_example_suite();

void add_suites(SRunner * sr)
{
    /* Add the tests to the runner here: */
    srunner_add_suite(sr, build_example_suite());
}

/****************************************************************************/

int main()
{
    int failed;

    SRunner * sr = srunner_create(suite_create("Main"));

    add_suites(sr);
    srunner_run_all(sr, CK_ENV);
    failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
