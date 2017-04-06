#include <stdlib.h>
#include <check.h>

Suite * build_main_suite()
{
    Suite * s = suite_create("Main");
    return s;
}

Suite * build_stack_suite();
Suite * build_world_suite();

int main()
{
    int failed;

    SRunner * sr = srunner_create(build_main_suite());
    srunner_add_suite(sr, build_stack_suite());
    srunner_add_suite(sr, build_world_suite());

    srunner_run_all(sr, CK_ENV);

    failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
