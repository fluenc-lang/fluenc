cmake_minimum_required(VERSION 3.10)

include(GNUInstallDirs)

project(fluenc-lang LANGUAGES C CXX)

find_package(LLVM REQUIRED CONFIG)
find_package(Immer CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)

file(GLOB_RECURSE SOURCE_FILES *.cpp *.h)

set(PUBLIC_HEADERS
	expression.hpp
	namespace.hpp
	ast.hpp
	indexed.hpp
	token_info.hpp
	utility.hpp
	comfy_stack.hpp
	ast_transformer.hpp
	ast/struct_field_node.hpp
	ast/struct_node.hpp
	ast/type_name_node.hpp
	ast/array_node.hpp
	ast/conditional_node.hpp
	ast/float_literal_node.hpp
	ast/terminator_node.hpp
	ast/expansion_node.hpp
	ast/global_node.hpp
	ast/nothing_node.hpp
	ast/local_node.hpp
	ast/string_literal_node.hpp
	ast/boolean_literal_node.hpp
	ast/character_literal_node.hpp
	ast/integral_literal_node.hpp
	ast/member_access_node.hpp
	ast/return_node.hpp
	ast/binary_node.hpp
	ast/unary_node.hpp
	ast/tuple_sink_node.hpp
	ast/instantiation_node.hpp
	ast/function_call_node.hpp
	ast/block_instruction_node.hpp
	ast/function_node.hpp
	ast/module_node.hpp
	ast/argument_node.hpp
	ast/extension_node.hpp
	iterators/all_iterator.hpp
	iterators/extremities_iterator.hpp
	iterators/index_iterator.hpp
)

add_library(${PROJECT_NAME} STATIC
	${SOURCE_FILES}
	${PUBLIC_HEADERS}
)

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

target_include_directories(${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>

	${LLVM_INCLUDE_DIR}
)

install(TARGETS fluenc-lang
	EXPORT fluenc-lang-config
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

foreach (FILE ${PUBLIC_HEADERS})
	get_filename_component(DIR ${FILE} DIRECTORY)
	install(FILES ${FILE} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fluenc-lang/${DIR})
endforeach()

install(EXPORT fluenc-lang-config
	FILE fluenc-lang-config.cmake
	NAMESPACE fluenc::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fluenc
)
