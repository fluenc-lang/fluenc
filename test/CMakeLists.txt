cmake_minimum_required(VERSION 3.10)

project(test LANGUAGES C CXX)

if (NOT ${FLUENC_BUILD_TESTS})
    return()
endif ()

find_package(Boost 1.67 REQUIRED COMPONENTS unit_test_framework)

file(GLOB_RECURSE SOURCE_FILES *.cpp *.h)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
)

target_link_libraries(${PROJECT_NAME}
    fluenc-lang
)

target_include_directories(${PROJECT_NAME} PRIVATE
    "."
    "../fluenc-lang"
)
