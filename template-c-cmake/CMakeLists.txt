# This file controls the cmake build system
#
# Additional options can be passed to cmake, either from project options
# defined with the options() command below, or from cmake's standard option.
# Most notably, this command will enable debugging symbols:
#
#     cd build && cmake -DCMAKE_BUILD_TYPE=Debug ..
#
# After cmake has completed, use the following commands:
#     make          => build the project
#     make doc      => build the documentation
#     make test     => run the tests
#
cmake_minimum_required (VERSION 2.8)
project (example C)
set(PROJECT_DESCRIPTION "Example project template")
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)

##############################################################################
# Settings

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra -pedantic -Werror -D_POSIX_C_SOURCE=200112L")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s")

##############################################################################
# Sources

# Look for header files in build directory (for config.h) and include dir
include_directories("${PROJECT_BINARY_DIR}" "include")

# List of sources for everything except main()
set(example_SRCS
    # Add your sources here
    src/example.c
)

# List of sources for testing suite
set(tests_SRCS
    # Add your tests here
    tests/example.c
)

##############################################################################
# Dependencies

# Add the libraries you use here. Example for SDL2:
#
#   find_package(SDL2 REQUIRED)
#   include_directories("${SDL2_INCLUDE_DIR}")
#   set(example_DEPS ${example_DEPS} ${SDL2_LIBRARY})

# Write the result of all our tests to config.h
configure_file("include/config.h.in" "config.h")

##############################################################################
# Targets

# Build all project sources
add_library(core STATIC ${example_SRCS})
add_executable(${CMAKE_PROJECT_NAME} src/main.c)

# Link all modules to the main() entry point, and add external libs
target_link_libraries(${CMAKE_PROJECT_NAME} core ${example_DEPS})

# Tell cmake to install project into /usr/local/bin
install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION bin)

##############################################################################
# Testing

# Try to find check unit test toolsuite, build ./runtests if found
find_package(Check)
if(CHECK_FOUND)
    enable_testing()

    add_executable(runtests ${tests_SRCS} tests/main.c)
    target_link_libraries(runtests core ${example_DEPS} ${CHECK_LIBRARIES})

    add_test("runtests" runtests)
endif()

##############################################################################
# Documentation

find_package(Doxygen)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/.doxygen"
               "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
add_custom_target(doc
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} -E tar "czf" "docs.tgz" docs
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating documentation"
)
