/** @file
 * Miscellaneous utility functions.
 */
#ifndef UTILS_H
#define UTILS_H

/** Get a random integer within an interval.
 *
 * Generated random number is corrected for linear distribution.
 * @param min inferior limit for random integer, inclusive.
 * @param max superior limit for random integer, inclusive.
 * @return a random integer in range `[min, max]`.
 * @author Function was written by
 *         [theJPster](http://stackoverflow.com/users/1336679/thejpster),
 *         who published it at http://stackoverflow.com/a/17554531/3212865
 * @copyright [CC-BY-SA 3.0](https://creativecommons.org/licenses/by-sa/3.0/)
 */
unsigned int rand_interval(unsigned int min, unsigned int max);

#endif
