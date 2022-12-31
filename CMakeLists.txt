cmake_minimum_required(VERSION 3.10)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)
FetchContent_Declare(_project_options URL https://github.com/aminya/project_options/archive/refs/tags/v0.26.1.zip)
FetchContent_MakeAvailable(_project_options)
include(${_project_options_SOURCE_DIR}/Index.cmake)

run_vcpkg(VCPKG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg)

project(fluenc LANGUAGES C CXX)

option(FLUENC_BUILD_TESTS "Build the tests for the project" ON)
option(FLUENC_BUILD_COMPILER "Build the compiler" ON)
option(FLUENC_BUILD_SANITIZERS "Build the compiler with ASan and UBSan support" OFF)
option(FLUENC_BUILD_EXAMPLES "Copy the examples to the build output" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
	add_compile_options(/Zc:__cplusplus /bigobj)
	add_compile_definitions(NOMINMAX)
	add_compile_definitions(NDEBUG)
else()
	add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_subdirectory("fluenc-lang")
add_subdirectory("fcc")
add_subdirectory("test")
add_subdirectory("incbin")
add_subdirectory("grammar")

add_custom_target(vcpkg SOURCES
	vcpkg.json
)

if(${FLUENC_BUILD_SANITIZERS})
    add_compile_options(-fsanitize=address,undefined)
    add_link_options(-fsanitize=address,undefined)
endif()
