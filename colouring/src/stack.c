/** @file
 * @copydoc stack.h
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

static const size_t initial_capacity = 16;  /**< Stack capacity for new stacks. */

struct stack {
    size_t  capacity;       /**< Stack capacity in item_size-objects */
    size_t  length;         /**< Current count of objects in the stack */
    size_t  item_size;      /**< Size of items in chars */
    void *  data;           /**< Storage area */
};

/****************************************************************************/

Stack * stack_create(size_t item_size)
{
    Stack * stack = malloc(sizeof(*stack));
    if (stack == NULL) { return NULL; }

    stack->capacity = initial_capacity;
    stack->length = 0;
    stack->item_size = item_size;
    if ((stack->data = malloc(initial_capacity * item_size)) == NULL) {
        free(stack);
        return NULL;
    }
    return stack;
}

void stack_destroy(Stack * stack)
{
    if (stack == NULL) { return; }
    free(stack->data);
    free(stack);
}

/****************************************************************************/

size_t stack_size(const Stack * stack)
{
    return stack->length;
}

bool stack_push(Stack * stack, const void * item)
{
    void * data;
    if (stack->length == stack->capacity) {
        data = realloc(stack->data, 2 * stack->capacity * stack->item_size);
        if (data == NULL) { return false; }
        stack->capacity *= 2;
        stack->data = data;
    }
    memcpy((char*)stack->data + stack->length * stack->item_size, item, stack->item_size);
    stack->length += 1;
    return true;
}

void stack_pop(Stack * stack, void * item)
{
    assert(stack->length > 0);
    memcpy(item, (char*)stack->data + (stack->length - 1) * stack->item_size,
           stack->item_size);
    stack->length -= 1;
}

void stack_clear(Stack * stack)
{
    void * data = realloc(stack->data, initial_capacity);
    stack->length = 0;
    if (data != NULL) {
        stack->capacity = initial_capacity;
        stack->data = data;
    }
}
