#
cmake_minimum_required (VERSION 3.0)
project(cubes VERSION 0.1 LANGUAGES CXX)
set(PROJECT_DESCRIPTION "Cubes demonstration")

cmake_policy(SET CMP0072 NEW)

##############################################################################
# Settings

# Where to find the FindSDL2.cmake script
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)

# How to call the xxd utility
set(XXD "xxd")

add_compile_options(-Wall -Wextra -Wsign-conversion -Wconversion -Wold-style-cast)

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

##############################################################################
# Dependencies

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

##############################################################################
# Targets

add_library(core STATIC ${cubes_SRCS})
target_compile_options(core PRIVATE -std=c++17 -ffast-math)
target_include_directories(core PRIVATE ${SDL2_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GLM_INCLUDE_DIRS})
add_library(shaders STATIC ${shaders_SRCS})

add_executable(${CMAKE_PROJECT_NAME} src/main.cxx)
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -std=c++17)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIR})
target_link_libraries(${CMAKE_PROJECT_NAME} core shaders ${SDL2_LIBRARY} OpenGL::OpenGL)

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION bin)
