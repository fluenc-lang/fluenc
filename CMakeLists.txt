cmake_minimum_required(VERSION 3.10)

project(fluenc LANGUAGES C CXX)

option(FLUENC_BUILD_TESTS "Build the tests for the project" OFF)
option(FLUENC_BUILD_COMPILER "Build the compiler" ON)
option(FLUENC_BUILD_SANITIZERS "Build the compiler with ASan and UBSan support" OFF)
option(FLUENC_BUILD_EXAMPLES "Copy the examples to the build output" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic)

add_subdirectory("fluenc-lang")
add_subdirectory("fcc")
add_subdirectory("test")

if(${FLUENC_BUILD_SANITIZERS})
    add_compile_options(-fsanitize=address,undefined)
    add_link_options(-fsanitize=address,undefined)
endif()
