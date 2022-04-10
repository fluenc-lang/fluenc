cmake_minimum_required(VERSION 3.10)

project(dz LANGUAGES CXX)

option(DZ_BUILD_TESTS "Build the tests for the project" OFF)
option(DZ_BUILD_COMPILER "Build the compiler" ON)
option(DZ_BUILD_SANITIZERS "Build the compiler with ASan and UBSan support" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic -O3)

if(${SZ_BUILD_SANITIZERS})
    add_compile_options(-fsanitize=address,undefined)
    add_link_options(-fsanitize=address,undefined)
endif()


set(PROJECT_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(PROJECT_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(PROJECT_TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test")
set(PROJECT_ANTLR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/antlr4-runtime")


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core REQUIRED)
find_library(LLVM13 LLVM-13 REQUIRED)
find_library(ANTLR4 antlr4-runtime REQUIRED)


include_directories(${PROJECT_HEADER_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories("/usr/include/antlr4-runtime")
include_directories("/usr/include/llvm-13")
include_directories("/usr/include/llvm-c-13")
include_directories(${PROJECT_TEST_DIR})

include(cmake/files.cmake)


execute_process(COMMAND ./tools/build.sh WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_library(dz-lang OBJECT ${LIB_SOURCE_FILES} ${LIB_HEADER_FILES})
target_link_libraries(dz-lang Qt5::Core antlr4-runtime LLVM-13)


if (${DZ_BUILD_TESTS})
    find_package(Qt5Test REQUIRED)

    add_executable(test ${TEST_SOURCE_FILES} ${TEST_HEADER_FILES})
    target_link_libraries(test dz-lang  Qt5::Core Qt5::Test  antlr4-runtime LLVM-13)
    target_include_directories(test PUBLIC ${PROJECT_TEST_DIR})
endif ()

if (${DZ_BUILD_COMPILER})
    add_executable(dz ${COMPILER_SOURCE_FILES} ${COMPILER_HEADER_FILES})
    target_link_libraries(dz dz-lang Qt5::Core  antlr4-runtime LLVM-13)
endif ()



file(GLOB_RECURSE ALL_FILES *.cpp *.h)

add_custom_target(clangformat WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMMAND clang-format -i ${ALL_FILES})
