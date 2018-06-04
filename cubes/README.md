Cubes
=======

This is a demonstration program for beginner-level 3D using modern OpenGL

Prerequisites
-------------

You need a working C++11-compliant compiler, with the following tools and packages:

* cmake
* xxd
* SDL2 development headers
* OpenGL development headers

On Debian-derived systems, you can install relevant packages using the
following command:

    sudo apt-get install build-essential cmake xxd libsdl2-dev libglm-dev mesa-common-dev

Building
--------

From the project root, run the following commands:

    cd build && cmake ..
    make

Or, to build with all optimisations, use this instead:

    cd build && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-march=native" ..
    make

Note
----

To avoid having to load files, yet let shaders be natural glsl files, they are embedded
as raw resources. Thus, CMakeLists.txt demonstrates a method to embed raw resources into
a C++ binary. It uses the `xxd` tool to generate a valid C++ source file, that looks like this:

    extern "C" const unsigned char original_filename[] = {
        0x00, 0x01, 0x02, 0x03, …, 0x41
    };
    extern "C" const unsigned int original_filename_len = 66;

Those files are the compiled and included at link stage. The include file
`include/resources.h` declares them manually, making them available to application code,
but it could be generated as well.

Authors
-------

* **Julien Hartmann** - https://github.com/spectras
