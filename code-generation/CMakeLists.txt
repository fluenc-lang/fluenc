cmake_minimum_required(VERSION 3.10)

include(GNUInstallDirs)

project(fluenc-code-generation LANGUAGES C CXX)

find_package(LLVM REQUIRED CONFIG)
find_package(Immer CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)

set(SOURCE_FILES
	emitter.cpp
	entry_point.cpp
	function_helper.cpp
	type_compatibility_calculator.cpp
	interop_helper.cpp
	value_helper.cpp
	ir_builder.cpp
	type.cpp
	utility.cpp

	values/expandable_value.cpp
	values/expanded_value.cpp
	values/tuple_value.cpp
	values/function_value.cpp
	values/post_lazy_value.cpp
	values/buffer_value.cpp
	values/expanded_lazy_value.cpp
	values/forwarded_value.cpp
	values/indexed_value.cpp
	values/pre_lazy_value.cpp
	values/named_value.cpp
	values/placeholder_value.cpp
	values/reference_value.cpp
	values/scalar_value.cpp
	values/string_value.cpp
	values/user_type_value.cpp
	values/without_value.cpp

	generators/array_iterator_generator.cpp
	generators/aggregate_iterator_generator.cpp
	generators/iterator_generator.cpp
	generators/string_iterator_generator.cpp

	types/opaque_pointer_type.cpp
	types/user_type.cpp
	types/default_prototype.cpp
	types/boolean_operator_set.cpp
	types/array_operator_set.cpp
	types/integer_operator_set.cpp
	types/float_operator_set.cpp
	types/without_operator_set.cpp
	types/string_operator_set.cpp
	types/byte_type.cpp
	types/int64_type.cpp
	types/int32_type.cpp
	types/uint32_type.cpp
	types/string_type.cpp
	types/float32_type.cpp
	types/boolean_type.cpp
	types/without_type.cpp
	types/array_type.cpp
	types/buffer_type.cpp
	types/void_type.cpp
	types/iterator_type.cpp
	types/expanded_type.cpp
	types/tuple_type.cpp
	types/structure_type.cpp
	types/aggregate_type.cpp
	types/any_type.cpp
	types/placeholder_type.cpp
	types/function_type.cpp
	types/proxy_type.cpp
	types/user_operator_set.cpp

	exceptions/invalid_type_exception.cpp
	exceptions/function_not_found_exception.cpp
	exceptions/ambiguous_function_exception.cpp
	exceptions/binary_type_mismatch_exception.cpp
	exceptions/invalid_argument_type_exception.cpp
	exceptions/invalid_function_pointer_type_exception.cpp
	exceptions/invalid_operator_exception.cpp
	exceptions/missing_default_value_exception.cpp
	exceptions/missing_field_exception.cpp
	exceptions/missing_tail_call_exception.cpp
	exceptions/missing_type_declaration_exception.cpp
	exceptions/unknown_type_exception.cpp
	exceptions/undeclared_identifier_exception.cpp
	exceptions/compiler_exception.cpp

	fragments/string_iterator_fragment.cpp
	fragments/array_continuation_fragment.cpp
	fragments/index_repeater_fragment.cpp
	fragments/pod_fragment.cpp
	fragments/aggregate_iterator_fragment.cpp
	fragments/index_injector_fragment.cpp
	fragments/array_iterator_fragment.cpp
	fragments/array_element_fragment.cpp
	fragments/aggregate_iterator_element_fragment.cpp
	fragments/default_fragment.cpp
	fragments/junction_fragment.cpp
	fragments/array_binary_fragment.cpp
	fragments/pre_evaluation_fragment.cpp
	fragments/post_evaluation_fragment.cpp
	fragments/stack_segment_fragment.cpp
	fragments/boolean_unary_fragment.cpp
	fragments/float_unary_fragment.cpp
	fragments/integer_unary_fragment.cpp
	fragments/string_unary_fragment.cpp
	fragments/array_unary_fragment.cpp
	fragments/user_unary_fragment.cpp
	fragments/reference_sink_fragment.cpp
	fragments/terminator_fragment.cpp
	fragments/array_sink_fragment.cpp
	fragments/index_sink_fragment.cpp
	fragments/expression_adapter_fragment.cpp
	fragments/continuation_fragment.cpp
	fragments/function_call_fragment.cpp

	metadata/tuple_value_metadata.cpp
	metadata/buffer_value_metadata.cpp
	metadata/expandable_value_metadata.cpp
	metadata/expanded_lazy_value_metadata.cpp
	metadata/expanded_value_metadata.cpp
	metadata/function_value_metadata.cpp
	metadata/indexed_value_metadata.cpp
	metadata/iterator_metadata.cpp
	metadata/lazy_value_metadata.cpp
	metadata/named_value_metadata.cpp
	metadata/placeholder_value_metadata.cpp
	metadata/reference_value_metadata.cpp
	metadata/scalar_value_metadata.cpp
	metadata/string_value_metadata.cpp
	metadata/user_type_value_metadata.cpp
	metadata/without_value_metadata.cpp
)

set(PUBLIC_HEADERS
	emitter.hpp
	entry_point.hpp
	comfy_stack.hpp
	base_value.hpp
	type.hpp

	types/default_prototype.hpp
	types/prototype_field.hpp

	exceptions/invalid_type_exception.hpp
)

set(PRIVATE_HEADERS
	function_helper.hpp
	type_compatibility_calculator.hpp
	interop_helper.hpp
	value_helper.hpp
	ir_builder.hpp
	fragment.hpp
	value_metadata.hpp
	utility.hpp
	transfer_strategy.hpp
	base_generator.hpp

	values/expandable_value.hpp
	values/expanded_value.hpp
	values/tuple_value.hpp
	values/function_value.hpp
	values/post_lazy_value.hpp
	values/buffer_value.hpp
	values/expanded_lazy_value.hpp
	values/forwarded_value.hpp
	values/indexed_value.hpp
	values/pre_lazy_value.hpp
	values/named_value.hpp
	values/placeholder_value.hpp
	values/reference_value.hpp
	values/scalar_value.hpp
	values/string_value.hpp
	values/user_type_value.hpp
	values/without_value.hpp

	types/opaque_pointer_type.hpp
	types/user_type.hpp
	types/operator_set.hpp
	types/boolean_operator_set.hpp
	types/array_operator_set.hpp
	types/integer_operator_set.hpp
	types/float_operator_set.hpp
	types/without_operator_set.hpp
	types/string_operator_set.hpp
	types/integer_type.hpp
	types/byte_type.hpp
	types/int64_type.hpp
	types/int32_type.hpp
	types/uint32_type.hpp
	types/string_type.hpp
	types/float32_type.hpp
	types/boolean_type.hpp
	types/without_type.hpp
	types/array_type.hpp
	types/builtin_type.hpp
	types/buffer_type.hpp
	types/void_type.hpp
	types/iterator_type.hpp
	types/expanded_type.hpp
	types/tuple_type.hpp
	types/structure_type.hpp
	types/aggregate_type.hpp
	types/any_type.hpp
	types/placeholder_type.hpp
	types/function_type.hpp
	types/proxy_type.hpp
	types/user_operator_set.hpp

	fragments/array_continuation_fragment.hpp
	fragments/index_repeater_fragment.hpp
	fragments/pod_fragment.hpp
	fragments/aggregate_iterator_fragment.hpp
	fragments/index_injector_fragment.hpp
	fragments/array_iterator_fragment.hpp
	fragments/binary_fragment.hpp
	fragments/unary_fragment.hpp
	fragments/array_element_fragment.hpp
	fragments/aggregate_iterator_element_fragment.hpp
	fragments/junction_fragment.hpp
	fragments/pre_evaluation_fragment.hpp
	fragments/post_evaluation_fragment.hpp
	fragments/default_fragment.hpp
	fragments/stack_segment_fragment.hpp
	fragments/reference_sink_fragment.hpp
	fragments/index_sink_fragment.hpp
	fragments/terminator_fragment.hpp
	fragments/array_sink_fragment.hpp
	fragments/expression_adapter_fragment.hpp
	fragments/continuation_fragment.hpp
	fragments/function_call_fragment.hpp
	fragments/string_iterator_fragment.hpp

	exceptions/function_not_found_exception.hpp
	exceptions/ambiguous_function_exception.hpp
	exceptions/binary_type_mismatch_exception.hpp
	exceptions/invalid_argument_type_exception.hpp
	exceptions/invalid_function_pointer_type_exception.hpp
	exceptions/invalid_operator_exception.hpp
	exceptions/missing_default_value_exception.hpp
	exceptions/missing_field_exception.hpp
	exceptions/missing_tail_call_exception.hpp
	exceptions/missing_type_declaration_exception.hpp
	exceptions/unknown_type_exception.hpp
	exceptions/undeclared_identifier_exception.hpp
	exceptions/compiler_exception.hpp

	metadata/tuple_value_metadata.hpp
	metadata/buffer_value_metadata.hpp
	metadata/expandable_value_metadata.hpp
	metadata/expanded_lazy_value_metadata.hpp
	metadata/expanded_value_metadata.hpp
	metadata/function_value_metadata.hpp
	metadata/indexed_value_metadata.hpp
	metadata/iterator_metadata.hpp
	metadata/lazy_value_metadata.hpp
	metadata/named_value_metadata.hpp
	metadata/placeholder_value_metadata.hpp
	metadata/reference_value_metadata.hpp
	metadata/scalar_value_metadata.hpp
	metadata/string_value_metadata.hpp
	metadata/user_type_value_metadata.hpp
	metadata/without_value_metadata.hpp

	generators/array_iterator_generator.hpp
	generators/aggregate_iterator_generator.hpp
	generators/iterator_generator.hpp
	generators/string_iterator_generator.hpp

	iterators/void_iterator.hpp
)

add_library(${PROJECT_NAME} STATIC
	${SOURCE_FILES}
	${PUBLIC_HEADERS}
	${PRIVATE_HEADERS}
)

target_compile_definitions(${PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")

target_link_libraries(${PROJECT_NAME} PUBLIC
	fluenc-lang

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
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>

	${LLVM_INCLUDE_DIR}
)

install(TARGETS ${PROJECT_NAME}
	EXPORT fluenc-code-generation-config
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

file(GLOB INCLUDE_FILES "include/*.h")

install(FILES ${INCLUDE_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fluenc/code_generation)

install(EXPORT fluenc-code-generation-config
	FILE fluenc-code-generation-config.cmake
	NAMESPACE fluenc::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fluenc
)
