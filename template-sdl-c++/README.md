Pure SDL2 Template
==================

This is a template to use for small C++ SDL2 projects.

Prerequisites
-------------

You need a working compiler, with the following tools and packages:

* cmake
* SDL2 development package
* git (optional but strongly recommended)

On Debian-derived systems, you can install relevant packages using the
following command:

    sudo apt-get install build-essential cmake libsdl2-dev git


Getting Started
---------------

Simply copy the whole template directory and rename it. Then you can initialize
the git repository for your project:

    git init
    git add .
    git commit -m "Deploy standard project template"

Congratulations, you have a working SDL2 project!

Building Your Project
---------------------

The template uses an *out-of-tree build*.

From the project root, run the following commands:

    cd build        # enter the build directory
    cmake ..        # detect libraries and configure the build environment
    make            # build the project

You should have your project binary in its first version. Let's try it:

    ./template-sdl-c++

Customizing your project
------------------------

To fully transform the template into your own project, follow those simple steps:

* Update `CMakeLists.txt`. At the very least:
    - Change the project name and description at the top of the file.
* Copy the [license you choose](https://choosealicense.com/) into the `LICENSE`
  file, and make sure to follow the LICENSE's instructions (usually this means
  adding your name and the date).
* Replace this file, `README.md` with your own instructions for your project!

A Few More Tips
---------------

**How do I build a debugging version to use with gdb / valgrind?**

  Set the build type when calling cmake:

      cmake -DCMAKE_BUILD_TYPE=Debug ..

**How do I build a release version, optimized and all?**

  Set the Release build type when calling cmake:

      cmake -DCMAKE_BUILD_TYPE=Release ..

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
