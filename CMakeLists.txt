cmake_minimum_required(VERSION 3.27)
project(Source2Schema)

include("cmake/global-flags.cmake")
if (MSVC)
    include("cmake/msvc-flags.cmake")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(Source2Schema STATIC ${SOURCES})

target_include_directories(Source2Schema PRIVATE headers)

target_compile_definitions(Source2Schema PRIVATE WIN32_LEAN_AND_MEAN) # Remove shit from Windows headers
