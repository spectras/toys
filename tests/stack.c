#include <check.h>
#include <stdbool.h>
#include <stdio.h>
#include "stack.h"

static const unsigned growth_test_count = 131072;

typedef struct {
    unsigned    foo;
    char *      bar;
} SampleItem;


START_TEST(test_stack_init)
{
    Stack * stack = stack_create(sizeof(SampleItem));
    ck_assert_ptr_ne(stack, NULL);
    ck_assert_uint_eq(stack_size(stack), 0);
    stack_destroy(stack);
}
END_TEST

START_TEST(test_stack_push_pop)
{
    SampleItem item;
    Stack * stack = stack_create(sizeof(SampleItem));
    ck_assert_ptr_ne(stack, NULL);

    ck_assert(stack_push(stack, &(SampleItem){1, "1111"}));
    ck_assert_uint_eq(stack_size(stack), 1);

    ck_assert(stack_push(stack, &(SampleItem){2, "2222"}));
    ck_assert_uint_eq(stack_size(stack), 2);

    stack_pop(stack, &item);
    ck_assert_uint_eq(stack_size(stack), 1);
    ck_assert_uint_eq(item.foo, 2);
    ck_assert_str_eq(item.bar, "2222");

    ck_assert(stack_push(stack, &(SampleItem){3, "3333"}));
    ck_assert_uint_eq(stack_size(stack), 2);

    stack_pop(stack, &item);
    ck_assert_uint_eq(stack_size(stack), 1);
    ck_assert_uint_eq(item.foo, 3);
    ck_assert_str_eq(item.bar, "3333");

    stack_pop(stack, &item);
    ck_assert_uint_eq(stack_size(stack), 0);
    ck_assert_uint_eq(item.foo, 1);
    ck_assert_str_eq(item.bar, "1111");

    stack_destroy(stack);
}
END_TEST

START_TEST(test_stack_growth)
{
    bool ok = true;
    unsigned i;
    Stack * stack = stack_create(sizeof(SampleItem));
    ck_assert_ptr_ne(stack, NULL);

    for (i = 0; i < growth_test_count; i += 1) {
        if (!stack_push(stack, &(SampleItem){i, NULL})) { ok = false; }
    }
    ck_assert_msg(ok, "at least one item was not properly pushed");
    ck_assert_uint_eq(stack_size(stack), growth_test_count);

    for (i = 0; i < growth_test_count; i += 1) {
        SampleItem item;
        stack_pop(stack, &item);
        if (item.foo != growth_test_count - i - 1) { ok = false; }
    }
    ck_assert_msg(ok, "at least one itme had incorrect value when popped");
    ck_assert_uint_eq(stack_size(stack), 0);

    stack_destroy(stack);
    fprintf(stderr, "finished\n");
}
END_TEST

/****************************************************************************/

Suite * build_stack_suite()
{
    Suite * s = suite_create("stack");
    TCase * tc = tcase_create("Core");
    tcase_add_test(tc, test_stack_init);
    tcase_add_test(tc, test_stack_push_pop);
    tcase_add_test(tc, test_stack_growth);

    suite_add_tcase(s, tc);
    return s;
}
