#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "config.h"

/****************************************************************************/

struct options {
    unsigned    verbosity;      /**< Logging verbosity, starting from 0 (quiet) */
    /*TODO add other options here */
};

bool parse_options(int argc, char * argv[], struct options * opts)
{
    char opt;
    /* Set default values for options */
    opts->verbosity  = 0;

    /* Read options from command line */
    while ((opt = getopt(argc, argv, "v")) != -1) {
        switch (opt) {
        case 'v':
            opts->verbosity += 1;
            break;
        default:
            fprintf(stderr, "Usage: %s [-v]\n", argv[0]);
            return false;
        }
    }
    return true;
}

/****************************************************************************/

int main(int argc, char * argv[])
{
    struct options options;
    if (!parse_options(argc, argv, &options)) { return 1; }

    /*TODO Add your code here */
    printf("Hello, World!\n");

    return EXIT_SUCCESS;
}
