# This file controls the cmake build system
# It is used like this:
#     cd build && cmake ..
#
# Additional options can be passed to cmake, either from project options
# defined with the options() command below, or from cmake's standard option.
# Most notably, this command will enable debugging symbols:
#     cd build && cmake -DCMAKE_BUILD_TYPE=Debug ..
#
# After cmake has completed, simply run  make  to build the project.
#
cmake_minimum_required (VERSION 3.3)
project (colouring VERSION 1.0 LANGUAGES C)

##############################################################################
# Settings

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)
add_compile_options(-std=c99 -Wall -Wextra -pedantic)

##############################################################################
# Sources

# Look for header files in build directory (for config.h) and include dir
include_directories("include")

# List of sources for everything except main()
set(colouring_SRCS
    src/colouring.c
    src/image.c
    src/stack.c
    src/utils.c
    src/world.c
)

# List of sources for testing suite
set(tests_SRCS
    tests/stack.c
    tests/world.c
)

# List of resourcs
set(colouring_RESOURCES
    background.png
    icon.png
)

##############################################################################
# Dependencies

find_package(SDL2 REQUIRED)
find_package(PNG REQUIRED)

##############################################################################
# Custom commands

set(RESOURCE_COMPILER xxd)
FOREACH(INPUT_FILE ${colouring_RESOURCES})
    set(OUTPUT_FILE ${INPUT_FILE}.c)
    ADD_CUSTOM_COMMAND(
        OUTPUT ${OUTPUT_FILE}
        COMMAND ${RESOURCE_COMPILER} -i resources/${INPUT_FILE} |sed 's/^unsigned/const unsigned/'> ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_FILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/resources/${INPUT_FILE}
        COMMENT "Embedding ${INPUT_FILE} into ${OUTPUT_FILE}"
    )
    LIST(APPEND RESOURCES ${OUTPUT_FILE})
ENDFOREACH()

##############################################################################
# Targets

# Build all project sources
add_library(core STATIC ${colouring_SRCS} ${RESOURCES})
target_include_directories(core PRIVATE ${PNG_INCLUDE_DIR})
target_include_directories(core PUBLIC ${SDL2_INCLUDE_DIR})

# Link all modules to the main() entry point, and add external libs
add_executable(colouring src/main.c)
target_compile_definitions(colouring PRIVATE _POSIX_C_SOURCE=200112L)
target_link_libraries(colouring -s core ${SDL2_LIBRARY} ${PNG_LIBRARY})

# Tell cmake to install colouring into /usr/local/bin
install(TARGETS colouring DESTINATION bin)

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
