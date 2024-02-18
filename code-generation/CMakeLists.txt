cmake_minimum_required(VERSION 3.10)

include(GNUInstallDirs)

project(fluenc-code-generation LANGUAGES C CXX)

find_package(LLVM REQUIRED CONFIG)
find_package(Immer CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)

set(SOURCE_FILES
	Emitter.cpp
	EntryPoint.cpp
	FunctionHelper.cpp
	Stack.cpp
	TypeCompatibilityCalculator.cpp
	InteropHelper.cpp
	ValueHelper.cpp
	IRBuilderEx.cpp
	Type.cpp
	CompilerException.cpp
	utility.cpp

	values/ExpandableValue.cpp
	values/ExpandedValue.cpp
	values/TupleValue.cpp
	values/ArrayValueGenerator.cpp
	values/FunctionValue.cpp
	values/Iteratable.cpp
	values/Iterator.cpp
	values/AggregateIteratorValueGenerator.cpp
	values/IteratorValueGenerator.cpp
	values/BufferValue.cpp
	values/ExpandedLazyValue.cpp
	values/ForwardedValue.cpp
	values/IndexedValue.cpp
	values/IteratorValueGeneratorProxy.cpp
	values/LazyValue.cpp
	values/NamedValue.cpp
	values/PlaceholderValue.cpp
	values/ReadOnlyIteratorValue.cpp
	values/ReferenceValue.cpp
	values/RenderedIteratorValue.cpp
	values/ScalarValue.cpp
	values/StringIteratable.cpp
	values/StringIteratableGenerator.cpp
	values/StringValue.cpp
	values/UserTypeValue.cpp
	values/WithoutValue.cpp

	types/OpaquePointerType.cpp
	types/UserType.cpp
	types/DefaultPrototype.cpp
	types/BooleanOperatorSet.cpp
	types/ArrayOperatorSet.cpp
	types/IntegerOperatorSet.cpp
	types/FloatOperatorSet.cpp
	types/WithoutOperatorSet.cpp
	types/StringOperatorSet.cpp
	types/IntegerType.cpp
	types/ByteType.cpp
	types/Int64Type.cpp
	types/Int32Type.cpp
	types/Uint32Type.cpp
	types/StringType.cpp
	types/Float32Type.cpp
	types/BooleanType.cpp
	types/WithoutType.cpp
	types/ArrayType.cpp
	types/BufferType.cpp
	types/VoidType.cpp
	types/IteratorType.cpp
	types/ExpandedType.cpp
	types/TupleType.cpp
	types/structure_type.cpp
	types/AggregateType.cpp
	types/AnyType.cpp
	types/PlaceholderType.cpp
	types/FunctionType.cpp
	types/ProxyType.cpp
	types/RangeType.cpp

	exceptions/InvalidTypeException.cpp
	exceptions/FunctionNotFoundException.cpp
	exceptions/AmbiguousFunctionException.cpp
	exceptions/BinaryTypeMismatchException.cpp
	exceptions/InvalidArgumentTypeException.cpp
	exceptions/InvalidFunctionPointerTypeException.cpp
	exceptions/InvalidOperatorException.cpp
	exceptions/MissingDefaultValueException.cpp
	exceptions/MissingFieldException.cpp
	exceptions/MissingTailCallException.cpp
	exceptions/MissingTypeDeclarationException.cpp
	exceptions/UnknownTypeException.cpp
	exceptions/UndeclaredIdentifierException.cpp

	fragments/ArrayContinuationFragment.cpp
	fragments/IteratableNode.cpp
	fragments/Pod.cpp
	fragments/AggregateIteratorValue.cpp
	fragments/ArrayValueProxy.cpp
	fragments/ArrayValue.cpp
	fragments/IteratorValue.cpp
	fragments/ArrayElementNode.cpp
	fragments/StaticNode.cpp
	fragments/AllocatorNode.cpp
	fragments/junction.cpp

	metadata/TupleValueMetadata.cpp
	metadata/BufferValueMetadata.cpp
	metadata/ExpandableValueMetadata.cpp
	metadata/ExpandedLazyValueMetadata.cpp
	metadata/ExpandedValueMetadata.cpp
	metadata/FunctionValueMetadata.cpp
	metadata/IndexedValueMetadata.cpp
	metadata/IteratorMetadata.cpp
	metadata/LazyValueMetadata.cpp
	metadata/NamedValueMetadata.cpp
	metadata/PlaceholderValueMetadata.cpp
	metadata/ReferenceValueMetadata.cpp
	metadata/ScalarValueMetadata.cpp
	metadata/StringValueMetadata.cpp
	metadata/UserTypeValueMetadata.cpp
	metadata/WithoutValueMetadata.cpp
)

set(PUBLIC_HEADERS
	Emitter.h
	EntryPoint.h
	DzResult.h
	Stack.h
	BaseValue.h
	Type.h
	CompilerException.h

	types/IPrototype.h
	types/PrototypeField.h

	exceptions/InvalidTypeException.h
)

set(PRIVATE_HEADERS
	FunctionHelper.h
	TypeCompatibilityCalculator.h
	InteropHelper.h
	ValueHelper.h
	IRBuilderEx.h
	fragment.hpp
	ValueMetadata.h
	utility.hpp
	ITransferStrategy.h

	values/ExpandableValue.h
	values/ExpandedValue.h
	values/TupleValue.h
	values/ArrayValueGenerator.h
	values/FunctionValue.h
	values/Iteratable.h
	values/Iterator.h
	values/AggregateIteratorValueGenerator.h
	values/IteratorValueGenerator.h
	values/BufferValue.h
	values/ExpandedLazyValue.h
	values/ForwardedValue.h
	values/ILazyValueGenerator.h
	values/IndexedValue.h
	values/IteratorValueGeneratorProxy.h
	values/LazyValue.h
	values/NamedValue.h
	values/PlaceholderValue.h
	values/ReadOnlyIteratorValue.h
	values/ReferenceValue.h
	values/RenderedIteratorValue.h
	values/ScalarValue.h
	values/StringIteratable.h
	values/StringIteratableGenerator.h
	values/StringValue.h
	values/UserTypeValue.h
	values/WithoutValue.h

	types/OpaquePointerType.h
	types/UserType.h
	types/IOperatorSet.h
	types/BooleanOperatorSet.h
	types/ArrayOperatorSet.h
	types/IntegerOperatorSet.h
	types/FloatOperatorSet.h
	types/WithoutOperatorSet.h
	types/StringOperatorSet.h
	types/IntegerType.h
	types/ByteType.h
	types/Int64Type.h
	types/Int32Type.h
	types/Uint32Type.h
	types/StringType.h
	types/Float32Type.h
	types/BooleanType.h
	types/WithoutType.h
	types/ArrayType.h
	types/BuiltinType.h
	types/BufferType.h
	types/VoidType.h
	types/IteratorType.h
	types/ExpandedType.h
	types/TupleType.h
	types/structure_type.hpp
	types/AggregateType.h
	types/AnyType.h
	types/PlaceholderType.h
	types/FunctionType.h
	types/ProxyType.h
	types/UserTypeField.h

	fragments/ArrayContinuationFragment.h
	fragments/IteratableNode.h
	fragments/Pod.h
	fragments/AggregateIteratorValue.h
	fragments/ArrayValueProxy.h
	fragments/ArrayValue.h
	fragments/IteratorValue.h
	fragments/binary_fragment.hpp
	fragments/unary_fragment.hpp
	fragments/ArrayElementNode.h
	fragments/StaticNode.h
	fragments/junction.hpp
	fragments/pre_evaluation.hpp
	fragments/post_evaluation.hpp
	fragments/AllocatorNode.h
	fragments/RenderedNode.h
	fragments/stack_segment.hpp

	exceptions/FunctionNotFoundException.h
	exceptions/AmbiguousFunctionException.h
	exceptions/BinaryTypeMismatchException.h
	exceptions/InvalidArgumentTypeException.h
	exceptions/InvalidFunctionPointerTypeException.h
	exceptions/InvalidOperatorException.h
	exceptions/MissingDefaultValueException.h
	exceptions/MissingFieldException.h
	exceptions/MissingTailCallException.h
	exceptions/MissingTypeDeclarationException.h
	exceptions/UnknownTypeException.h
	exceptions/UndeclaredIdentifierException.h

	metadata/TupleValueMetadata.h
	metadata/BufferValueMetadata.h
	metadata/ExpandableValueMetadata.h
	metadata/ExpandedLazyValueMetadata.h
	metadata/ExpandedValueMetadata.h
	metadata/FunctionValueMetadata.h
	metadata/IndexedValueMetadata.h
	metadata/IteratorMetadata.h
	metadata/LazyValueMetadata.h
	metadata/NamedValueMetadata.h
	metadata/PlaceholderValueMetadata.h
	metadata/ReferenceValueMetadata.h
	metadata/ScalarValueMetadata.h
	metadata/StringValueMetadata.h
	metadata/UserTypeValueMetadata.h
	metadata/WithoutValueMetadata.h

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
