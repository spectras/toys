#
cmake_minimum_required (VERSION 2.8)
project(cubes CXX)
set(PROJECT_DESCRIPTION "Cubes demonstration")
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)

##############################################################################
# Settings

# Where to find the FindSDL2.cmake script
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)

# Set additional compile flags:
# -std=c++11    -> set c++ dialect
# -ffast-math   -> tell the compiler it can perform optimisations at the expense of precision.
#                  Usually a good idea in 3D programs
# the rest      -> enable many warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -ffast-math -Wall -Wextra -pedantic -Werror")

# How to call the xxd utility
set(XXD "xxd")

##############################################################################
# Sources

include_directories("include")

# List of sources for everything except main()
set(cubes_SRCS
    src/gl/Shader.cxx
    src/gl/Vertex.cxx
    src/gl/common.cxx
    src/Application.cxx
)

##############################################################################
# Dependencies

find_package(SDL2 REQUIRED)
include_directories("${SDL2_INCLUDE_DIR}")
set(cubes_DEPS ${cubes_DEPS} ${SDL2_LIBRARY})

find_package(OpenGL REQUIRED)
include_directories("${OPENGL_INCLUDE_DIR}")
set(cubes_DEPS ${cubes_DEPS} ${OPENGL_gl_LIBRARY})

find_package(glm REQUIRED)
include_directories("${GLM_INCLUDE_DIRS}")

##############################################################################
# Targets

# Build all project sources
add_library(core STATIC ${cubes_SRCS})
add_executable(${CMAKE_PROJECT_NAME} src/main.cxx)

# Embed shaders - see README
file(GLOB_RECURSE shaders RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "shaders/*")
foreach(INPUT_FILE ${shaders})
    set(OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/${INPUT_FILE}.cxx")
    get_filename_component(OUTPUT_DIRECTORY ${OUTPUT_FILE} DIRECTORY)
    file(MAKE_DIRECTORY ${OUTPUT_DIRECTORY})
    add_custom_command(
        OUTPUT ${OUTPUT_FILE}
        COMMAND ${XXD} -i ${INPUT_FILE} | sed s/^unsigned/extern\ \\\"C\\\"\ const\ unsigned/ > ${OUTPUT_FILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_FILE}
    )
    list(APPEND shaders_SRCS ${OUTPUT_FILE})
endforeach()
add_library(shaders STATIC ${shaders_SRCS})

# Link all modules to the main() entry point, and add external libs
target_link_libraries(${CMAKE_PROJECT_NAME} core shaders ${cubes_DEPS})

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION bin)
