cmake_minimum_required(VERSION 3.10)

project(fcc LANGUAGES C CXX)

if (NOT ${FLUENC_BUILD_COMPILER})
	return()
endif ()

find_package(LLD REQUIRED CONFIG)
find_package(tomlplusplus CONFIG REQUIRED)
find_package(LibArchive REQUIRED)
find_package(Clang REQUIRED)

find_path(CPP_PEGLIB_INCLUDE_DIRS "peglib.h")

file(GLOB_RECURSE SOURCE_FILES *.cpp *.h)

add_executable(${PROJECT_NAME}
	${SOURCE_FILES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	fluenc-lang
	grammar

	tomlplusplus::tomlplusplus
	LibArchive::LibArchive
)

if (${CLANG_LINK_CLANG_DYLIB})
	target_link_libraries(${PROJECT_NAME} PRIVATE
		clang-cpp
	)

    install(IMPORTED_RUNTIME_ARTIFACTS clang-cpp)
else()
	target_link_libraries(${PROJECT_NAME} PRIVATE
		clangFrontend
		clangAst
		clangBasic
		clangLex
		clangParse
		clangCodeGen
		clangSema
	)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
	"."

	${LLD_INCLUDE_DIRS}
	${CPP_PEGLIB_INCLUDE_DIRS}
)

install(TARGETS fcc COMPONENT tomlplusplus::tomlplusplus)
install(IMPORTED_RUNTIME_ARTIFACTS clang)
