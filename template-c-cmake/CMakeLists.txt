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
cmake_minimum_required (VERSION 3.0)
project (example VERSION 0.1 LANGUAGES C)
set(PROJECT_DESCRIPTION "Example project template")

##############################################################################
# Settings

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)
add_compile_options(-Wall -Wextra -Werror)

##############################################################################
# Sources

# Project-wide include paths
include_directories("include")

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

# Write the result of all our tests to config.h and ensure all targets see it
configure_file("include/config.h.in" "config.h")
include_directories("${PROJECT_BINARY_DIR}")

##############################################################################
# Targets

# Build all project sources
add_library(core STATIC ${example_SRCS})
target_compile_options(core PRIVATE -std=c99)

# Generate executable
add_executable(${CMAKE_PROJECT_NAME} src/main.c)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE _POSIX_C_SOURCE=200112L)
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -std=c99)
target_link_libraries(${CMAKE_PROJECT_NAME} core)

# Tell cmake to install project into /usr/local/bin
install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION bin)

##############################################################################
# Testing

# Try to find check unit test toolsuite, build ./runtests if found
find_package(Check)
if(CHECK_FOUND)
    enable_testing()

    add_executable(runtests ${tests_SRCS} tests/main.c)
    target_link_libraries(runtests core ${CHECK_LIBRARIES})

    add_test("runtests" runtests)
endif()

##############################################################################
# Documentation

find_package(Doxygen)
if (DOXYGEN_FOUND)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/.doxygen"
                "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -E tar "czf" "docs.tgz" docs
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating documentation"
    )
endif()
