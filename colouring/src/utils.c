/** @file
 * @copydoc utils.h
 */
#include <stdlib.h>
#include "utils.h"

/* Function code taken from http://stackoverflow.com/a/17554531/3212865
 * Authored by [theJPster](http://stackoverflow.com/users/1336679/thejpster)
 * Licensed under [CC-BY-SA 3.0](https://creativecommons.org/licenses/by-sa/3.0/)
 */
unsigned int rand_interval(unsigned int min, unsigned int max)
{
    unsigned int r;
    const unsigned range = 1 + max - min;
    const unsigned buckets = RAND_MAX / range;
    const unsigned limit = buckets * range;

    do
    {
        r = rand();
    } while (r >= limit);

    return min + (r / buckets);
}
