cmake_minimum_required(VERSION 3.10)

project(fcc LANGUAGES C CXX)

if (NOT ${FLUENC_BUILD_COMPILER})
    return()
endif ()

find_package(LLD REQUIRED CONFIG)

file(GLOB_RECURSE SOURCE_FILES *.cpp *.h)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
)

target_link_libraries(${PROJECT_NAME}
    fluenc-lang
    lldELF
)

target_include_directories(${PROJECT_NAME} PRIVATE
    "."

    ${LLD_INCLUDE_DIRS}
)
