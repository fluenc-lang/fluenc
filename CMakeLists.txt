cmake_minimum_required(VERSION 3.10)

project(fluenc LANGUAGES CXX)

option(FLUENC_BUILD_TESTS "Build the tests for the project" OFF)
option(FLUENC_BUILD_COMPILER "Build the compiler" ON)
option(FLUENC_BUILD_SANITIZERS "Build the compiler with ASan and UBSan support" OFF)
option(FLUENC_BUILD_EXAMPLES "Copy the examples to the build output" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic)

if(${FLUENC_BUILD_SANITIZERS})
    add_compile_options(-fsanitize=address,undefined)
    add_link_options(-fsanitize=address,undefined)
endif()

if(${FLUENC_BUILD_EXAMPLES})
	execute_process(COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/examples ${CMAKE_CURRENT_BINARY_DIR}/examples)
endif()

set(PROJECT_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(PROJECT_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(PROJECT_TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test")
set(PROJECT_ANTLR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/antlr4-runtime")


find_library(LLVM13 LLVM-13 REQUIRED)
find_library(ANTLR4 antlr4-runtime REQUIRED)

find_package(LLD REQUIRED)


include_directories(${PROJECT_HEADER_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories("/usr/include/antlr4-runtime")
include_directories("/usr/include/llvm-13")
include_directories("/usr/include/llvm-c-13")

include(cmake/files.cmake)


execute_process(COMMAND ./tools/build.sh WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_library(fluenc-lang OBJECT ${LIB_SOURCE_FILES} ${LIB_HEADER_FILES})
target_link_libraries(fluenc-lang antlr4-runtime LLVM-13)


if (${FLUENC_BUILD_TESTS})
    find_package(Boost 1.67 REQUIRED COMPONENTS unit_test_framework)

    add_executable(test ${TEST_SOURCE_FILES} ${TEST_HEADER_FILES})
    target_link_libraries(test fluenc-lang antlr4-runtime LLVM-13)
    target_include_directories(test PUBLIC ${PROJECT_TEST_DIR})
endif ()

if (${FLUENC_BUILD_COMPILER})
    add_executable(fcc ${COMPILER_SOURCE_FILES} ${COMPILER_HEADER_FILES})
    target_link_libraries(fcc fluenc-lang antlr4-runtime LLVM-13 lldELF)
endif ()



file(GLOB_RECURSE ALL_FILES *.cpp *.h)

add_custom_target(clangformat WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMMAND clang-format -i ${ALL_FILES})
