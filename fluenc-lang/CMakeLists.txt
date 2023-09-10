cmake_minimum_required(VERSION 3.10)

project(fluenc-lang LANGUAGES C CXX)

find_package(LLVM REQUIRED CONFIG)
find_package(Immer CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)

file(GLOB_RECURSE SOURCE_FILES *.cpp *.h)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_compile_definitions(${PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")

target_link_libraries(${PROJECT_NAME} PUBLIC
    immer
    range-v3

    fmt::fmt
)

if (${LLVM_LINK_LLVM_DYLIB})
	target_link_libraries(${PROJECT_NAME} PUBLIC
		LLVM
	)

    install(IMPORTED_RUNTIME_ARTIFACTS LLVM)
else()
	target_link_libraries(${PROJECT_NAME} PUBLIC
		${LLVM_AVAILABLE_LIBS}
	)
endif()

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
