/** @file
 * Generic array-based stack implementation.
 *
 * Implements a regular, array-backed, value-based LIFO stack.
 * It contains items with fixed size, stored by value.
 *
 * Growth is automatic, with asymptotic complexity in `O(log N)`.
 */
#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stddef.h>

/** Arbitrary-content stack */
typedef struct stack Stack;

/****************************************************************************/
/** @name Stack Lifetime
 *  @{
 */

/** Create a new stack
 * @param item_size The size of an object in chars
 * @return The newly created stack, or `NULL` on failure. The stack must be
 *         freed with stack_destroy().
 */
Stack * stack_create(size_t item_size);

/** Destroy a stack
 * @param[in] stack The stack to destroy. It is safe to pass `NULL` to this
 *                  function.
 */
void    stack_destroy(Stack * stack);

/** @}*/

/****************************************************************************/
/** @name Stack Manipulation
 *  @{
 */

/** Get current stack size
 * @param[in] stack The stack.
 * @return The count of items stored on the stack.
 */

size_t  stack_size(const Stack * stack);

/** Push an item onto the stack
 *
 * The item is copied onto the stack, and can be safely discarded once this
 * function returns. Stack will grow if capacity is exhausted.
 * @param[in] stack The stack.
 * @param[in] item Address of the item to store.
 * @return `true` on success, `false` on memory error.
 */
bool    stack_push(Stack * stack, const void * item);

/** Pops an item from the stack
 *
 * This function cannot fail.
 * @param[in] stack The stack.
 * @param[out] item Address where to copy the item to. This address must be
 *                  able to hold at least `stack->item_size` chars.
 * @pre `stack_size(stack) > 0`
 */
void    stack_pop(Stack * stack, void * item);

/** Clear the whole stack
 *
 * All items on the stack are discarded, and an attempt to shrink its capacity
 * down to its initial value is made. If it fails, capacity remains unchanged
 * but the stack is still valid.
 * @param[in] stack The stack.
 * @post `stack_size(stack) == 0`
 */
void    stack_clear(Stack * stack);

/** @} */

#endif
