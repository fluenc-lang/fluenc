cmake_minimum_required(VERSION 3.10)

project(fluenc-lang LANGUAGES C CXX)

find_package(LLVM REQUIRED CONFIG)

file(GLOB_RECURSE SOURCE_FILES *.cpp *.h)

add_library(${PROJECT_NAME} OBJECT ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PUBLIC LLVM)
target_compile_definitions(${PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")

target_link_directories(${PROJECT_NAME} PUBLIC
    ${LLVM_LIBRARY_DIR}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    "."
)

target_include_directories(${PROJECT_NAME} PUBLIC
    "include"

    ${LLVM_INCLUDE_DIR}

    "/usr/include/llvm-c-13"
)
