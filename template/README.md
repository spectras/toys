Example
=======

This is a template to use for small to medium development projects.

Prerequisites
-------------

You need a working compiler, with the following tools and packages:

* cmake
* check
* doxygen
* git (optional but strongly recommended)

On Debian-derived systems, you can install relevant packages using the
following command:

    sudo apt-get install build-essential cmake check doxygen git


Getting Started
---------------

Simply copy the whole template directory and rename it. Then you can initialize
the git repository for your project:

    git init
    git add .
    git commit -m "Deploy standard project template"

Congratulations, you have a working C project!

Here is a brief description of how the template works:

* **[`build`](build):** an empty directory. This is where you will compile
  your project.
* **[`CMakeModules`](CMakeModules):** when using external libraries that
  CMake (see below) does not know, you must give it custom modules that you
  usually find on the internet. This directory is where you put them.
* **[`include`](include):** put all your header files in this directory.
  They will be found under the normal name. For instance, to include
  `include/foo/bar.h`, you will simply `#include "foo/bar.h"`.
* **[`include/config.h.in`](include/config.h.in):** this special header
  allows CMake (see below) to pass the result of its system configuration
  to your C code. This can be useful, for instance, if you want to enable
  or disable some functions depending on whether a special library is
  installed on the system.
* **[`src`](src):** put all your source files in this directory.
* **[`tests`](tests):** put all your unit tests in this directory.
* **[`CMakeLists.txt`](CMakeLists.txt):** The template uses
  [CMake](https://cmake.org/) to control the build process. This file controls
  CMake itself. More on CMake below.
* **[`.doxygen`](.doxygen):** This file controls doxygen settings. You don't
  have to change it, CMake will fill appropriate values depending on what you
  have in your `CMakeLists.txt`.
* **[`LICENSE`](LICENSE):** Your code's license.
* **`README.md`:** This file! Once you have read all this, replace it with the
  README for your project. You can use
  [Markdown](https://guides.github.com/features/mastering-markdown/) in it.


Building Your Project
---------------------

The template uses a process known as an *out-of-tree build*. It means that your
main directories are left completely untouched when you compile. It makes
it easier to clean up (simply `rm -rf build/*`) and prevents mistakes.

Let's build your project. From the project root, run the following commands:

    cd build        # enter the build directory
    cmake ..        # detect libraries and configure the build environment
    make            # build the project
    make doc        # build the project documentation

You should have your project binary in its first version. Let's try it:

    ./example
    Hello, World!

Unit tests should also be compiled:

    ./runtests
    Running suite(s): Main
     example
    100%: Checks: 1, Failures: 0, Errors: 0

Customizing your project
------------------------

To fully transform the template into your own project, follow those simple steps:

* Remove example files: `include/example.h`, `src/example.c` and `tests/example.c`.
* Update `CMakeLists.txt`. At the very least:
    - Change the project name and description at the top of the file.
    - Rename all variables that start with `example_*` so they start
      with your project's name.
    - Remove example files from the list of sources.
* Copy the [license you choose](https://choosealicense.com/) into the `LICENSE`
  file, and make sure to follow the LICENSE's instructions (usually this means
  adding your name and the date).
* Replace this file, `README.md` with your own instructions for your project!

That's it! Don't forget to re-run `cmake ..` everytime you modify
`CMakeLists.txt`.

And don't forget to commit the changes using `git`.

More about cmake and CMakeLists.txt
-----------------------------------

[CMake](https://cmake.org/) is a cross-platform toolset. It is used to build
and package software projects. Using it over simple Makefiles has many
advantages:

* Simpler, more intuitive syntax.
* Cross-platform: cmake will generate the appropriate files depending on the
  platform: Makefile, MSVS project, …
* Customizable builds: cmake can detect the presence and the appropriate
  options to compile and link against libraries on a wide range of systems.
  It is even possible to change the build process depending on availabe
  libraries. For instance, make your project read PNG images when libpng
  is detected, and stick to bmp otherwise.
* Automatic dependencies. CMake reads source files, calculates dependencies
  automatically and adds them to the Makefile.
* Easier packaging. Most Linux packaging systems know cmake and can generate
  deb or rpm packages for a CMake project with minimal effort.

So, how to use it? First, read `CMakeLists.txt`. It is the file that controls
cmake. In a complex project, there will be several `CMakeLists.txt` arranged
in a tree, but this template only has one. It is well documented and shows
the most common commands and variables.

The basic idea is CMake is controlled through:

* [**commands**](https://cmake.org/cmake/help/v3.0/manual/cmake-commands.7.html),
  that tell cmake to find things or explains how to build some
  pieces of the project. For instance, `find_package(SDL2)`
* **targets**, that tell cmake what the final results should be. They are created
  with special commands `add_executable`, `add_library` and `add_custom_target`.
* [**variables**](https://cmake.org/cmake/help/v3.0/manual/cmake-variables.7.html),
  that modify the behavior of cmake. They are modified with the command `set`.
  For instance: `set(CMAKE_PROJECT_NAME "example")`. Many special variables are
  recognized by cmake.

In this template, the provided `CMakeLists.txt` builds the project in three parts:

* All source code is compiled into a static library named `libcore`.
* Then this static library is linked to `src/main.c` to generate the final binary.
* Then the static library is linked to `tests/main.c` to generate the `runtests`
  binary.

Therefore, to add new source files to your project, you just have to add
them to either `yourproject_SRCS`, while unit test sources go in `tests_SRCS`.

Don't forget to re-run `cmake` everytime you change `CMakeLists.txt`.

A Few More Tips
---------------

**How do I build a debugging version to use with gdb / valgrind?**

  Set the build type when calling cmake:

      cmake -DCMAKE_BUILD_TYPE=Debug ..

**How do I build a release version, optimized and all?**

  Set the Release build type when calling cmake:

      cmake -DCMAKE_BUILD_TYPE=Release ..

**My project is C++, can I use the template?**

  Yes! At the top of `CMakeLists.txt`, in the `project()` command, change
  the project language from `C` to `CXX`.<br>
  You will probably want to go through the rest of `CMakeLists.txt` to
  change variables configuring C code so they configure C++ instead. For
  instance, `CMAKE_C_FLAGS` becomes `CMAKE_CXX_FLAGS`.

Authors
-------

* **Julien Hartmann** - https://github.com/spectras

License
-------

This template project has a double licensing:

* The template itself is licensed under the MIT License. Mostly, this means you
  must retain copyright information and license, but you are otherwise
  free to modify, copy, redistribute it or sell it. See the [LICENSE](LICENSE)
  file for details.
* Once you have customized your project as described in this README.md, you
  are free to use the resulting project in any way you want. Keeping copyright
  or mentionning the template's author is appreciated, but not required.
