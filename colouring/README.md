Colouring Game
=====

This project implements a small game used as practice subject at
the ENSIEE engineering school.

Getting Started
---------------

These instructions will get you a copy of the project up and
running on your local machine for testing purposes.

Prerequisites
-------------

You need a working C99-compliant compiler, with the following packages:

* cmake
* check (optional)
* libpng
* libSDL2

On Debian-derived systems, you can install relevant packages using the
following command:

    sudo apt-get install build-essential cmake check libpng12-dev libsdl2-dev

Add `doxygen` on top of that if you want the documentation.

Building and Testing
--------------------

From the project root, run the following commands:

    cd build && cmake ..    # Configure the build environment
    make                    # Build the game
    make doc                # Build the documentation

First command will detect packages and setup the build environment.
It can be passed additional options. Most notably, add
`-DCMAKE_BUILD_TYPE=Debug` to get debugging symbols in the generated
binary, or `-DCMAKE_BUILD_TYPE=Release` to get an optimized, stripped
binary.

Second command will build the project into the ``build`` directory.
Pass it `VERBOSE=1` if you want to see the actual commands run to
build the project.

You should be able to run the test suite, by typing the following command:

    ./runtests

The output should look like this:

    100%: Checks: 6, Failures: 0, Errors: 0


Running
-------

Run the program from the build directory:

    ./colouring

It recognizes the following options:

* <code><b>-n</b> <em>colors</em></code>: Set the number of colors in game world.
  Defaults to 6.
* <code><b>-s</b> <em>seed</em></code>: Force the seed for the world generator.
  This can be used to replay the same world several times, either for debugging
  purposes, or to see if you can improve your score!
* <code><b>-t</b> <em>turns</em></code>: Set the number of turns in the game.
  If the world is not completely flooded at the end of that turn, the game is lost.
  If this option is not specified, a default is chosen depending on the number
  of colors.
* <code><b>``-v``</b></code>: increase verbosity level.

In addition, an optional argument can be passed to set the grid size. It must
be in form <code><em>width</em><b>x</b><em>height</em></code>. If not
specified, it defaults to `20x15`.

Have fun, and good luck for your homework assignment!

Authors
-------

* **Julien Hartmann** - *Initial work* - [spectras].

See also the list of [contributors](https://github.com/spectras/toys/contributors) who participated in this project.

License
-------

This project is licensed under the MIT License - see the [LICENSE] file for details

[spectras]: https://github.com/spectras
[LICENSE]: https://github.com/spectras/keyleds/toys/master/colouring/LICENSE
[LICENSE]: @ref LICENSE
