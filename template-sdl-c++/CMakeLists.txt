#
cmake_minimum_required (VERSION 3.3)
project (template-sdl-c++ VERSION 1.0 LANGUAGES CXX)

##############################################################################
# Settings

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)
add_compile_options(-Wall -Wextra -Wsign-conversion -Wconversion -Wold-style-cast)

##############################################################################
# Sources

set(SRCS
    src/Application.cxx
    src/main.cxx
)

##############################################################################
# Dependencies

find_package(SDL2 REQUIRED)

##############################################################################
# Targets

add_executable(${CMAKE_PROJECT_NAME} ${SRCS})
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -std=c++17 -ffast-math)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE "include" ${SDL2_INCLUDE_DIR})
target_link_libraries(${CMAKE_PROJECT_NAME} ${SDL2_LIBRARY})

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION bin)
