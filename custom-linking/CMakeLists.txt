#
cmake_minimum_required (VERSION 3.0)
project(custom-linking VERSION 0.1 LANGUAGES CXX)
set(PROJECT_DESCRIPTION "Custom linker script demonstration")

##############################################################################
# Settings

add_compile_options(-Wall -Wextra -Wsign-conversion -Wconversion -Wold-style-cast)

##############################################################################
# Sources

set(SRCS
    src/main.cxx
    src/module1.cxx
    src/module2.cxx
    src/module3.cxx
)

set(LINKER_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/linker.script)

##############################################################################
# Feature checks

include(CheckCXXSourceCompiles)
check_cxx_source_compiles("int __attribute__((used)) foo; int main() {return 0;}"
                          HAS_ATTRIBUTE_USED)
check_cxx_source_compiles("int __attribute__((section (\".foo\"))) foo; int main() {return 0;}"
                          HAS_ATTRIBUTE_SECTION)
if (NOT HAS_ATTRIBUTE_SECTION)
    message(SEND_ERROR "Support for section attribute is required for this program")
endif (NOT HAS_ATTRIBUTE_SECTION)

##############################################################################
# Targets

configure_file("include/config.h.in" "config.h")
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(${CMAKE_PROJECT_NAME} ${SRCS})
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -std=c++17)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include)
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    LINK_FLAGS ${LINKER_SCRIPTS}
    LINK_DEPENDS ${LINKER_SCRIPTS}
)
