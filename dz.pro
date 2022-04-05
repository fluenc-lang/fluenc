QT += testlib

CONFIG += DEBUG c++17

SOURCES += \
	AllIterator.cpp \
	nodes/BlockInstruction.cpp \
	nodes/Continuation.cpp \
	nodes/Expansion.cpp \
	nodes/ExportedFunctionTerminator.cpp \
	nodes/FunctionCallProxy.cpp \
	nodes/IntegralLiteral.cpp \
	nodes/ImportedFunction.cpp \
	nodes/Instantiation.cpp \
	nodes/MemberAccess.cpp \
	nodes/StringLiteral.cpp \
	nodes/FunctionCall.cpp \
	nodes/Global.cpp \
	nodes/Local.cpp \
	nodes/Return.cpp \
	nodes/ArrayContinuation.cpp \
	nodes/ArrayElement.cpp \
	nodes/ArraySink.cpp \
	nodes/Binary.cpp \
	nodes/BlockStackFrame.cpp \
	nodes/BooleanLiteral.cpp \
	nodes/CharacterLiteral.cpp \
	CompilerException.cpp \
	nodes/Conditional.cpp \
	DefaultPrototypeProvider.cpp \
	DzArgument.cpp \
	DzTupleArgument.cpp \
	DzTypeName.cpp \
	nodes/EmptyArray.cpp \
	EntryPoint.cpp \
	nodes/ExportedFunction.cpp \
	nodes/Function.cpp \
	FunctionNotFoundException.cpp \
	FunctionTypeName.cpp \
	IRBuilderEx.cpp \
	IndexIterator.cpp \
	nodes/IndexSink.cpp \
	InteropHelper.cpp \
	IteratorStorage.cpp \
	nodes/Junction.cpp \
	nodes/LazyEvaluation.cpp \
	nodes/LazySink.cpp \
	Node.cpp \
	nodes/ReferenceSink.cpp \
	Stack.cpp \
	nodes/StackSegment.cpp \
	nodes/TaintedSink.cpp \
	nodes/Terminator.cpp \
	Type.cpp \
	UndeclaredIdentifierException.cpp \
	UnknownTypeException.cpp \
	Utility.cpp \
	ValueHelper.cpp \
	VisitorV1.cpp \
	VisitorV4.cpp \
	WithPrototypeProvider.cpp \
	main.cpp \
	antlr4-runtime/dzBaseVisitor.cpp \
	antlr4-runtime/dzLexer.cpp \
	antlr4-runtime/dzParser.cpp \
	antlr4-runtime/dzVisitor.cpp \
	nodes/Namespace.cpp \
	types/AnyType.cpp \
	types/ArrayType.cpp \
	types/BooleanType.cpp \
	types/ByteType.cpp \
	types/FunctionType.cpp \
	types/Int32Type.cpp \
	types/Int64Type.cpp \
	types/Prototype.cpp \
	types/PrototypeField.cpp \
	types/PrototypeFieldEmbryo.cpp \
	types/StringType.cpp \
	types/TupleType.cpp \
	types/Uint32Type.cpp \
	types/UserType.cpp \
	types/UserTypeField.cpp \
	types/VoidType.cpp \
	types/WithPrototype.cpp \
	types/WithoutType.cpp \
	values/ArrayValue.cpp \
	values/ArrayValueGenerator.cpp \
	values/DependentValue.cpp \
	values/ExpandableValue.cpp \
	values/FunctionValue.cpp \
	values/IteratorValue.cpp \
	values/IteratorValueGenerator.cpp \
	values/LazyValue.cpp \
	values/NamedValue.cpp \
	values/ReferenceValue.cpp \
	values/TaintedValue.cpp \
	values/TupleValue.cpp \
	values/TypedValue.cpp \
	values/IndexedValue.cpp \
	values/UserTypeValue.cpp \
	values/WithoutValue.cpp

HEADERS += \
	AllIterator.h \
	nodes/BlockInstruction.h \
	nodes/Continuation.h \
	nodes/Expansion.h \
	nodes/ExportedFunctionTerminator.h \
	nodes/FunctionCallProxy.h \
	nodes/IntegralLiteral.h \
	nodes/ImportedFunction.h \
	nodes/Instantiation.h \
	nodes/MemberAccess.h \
	nodes/StringLiteral.h \
	nodes/FunctionCall.h \
	nodes/Global.h \
	nodes/Local.h \
	nodes/Return.h \
	nodes/ArrayContinuation.h \
	nodes/ArrayElement.h \
	nodes/ArraySink.h \
	nodes/Binary.h \
	nodes/BlockStackFrame.h \
	nodes/BooleanLiteral.h \
	nodes/Callable.h \
	nodes/CharacterLiteral.h \
	CompilerException.h \
	nodes/Conditional.h \
	DefaultPrototypeProvider.h \
	DzArgument.h \
	DzBaseArgument.h \
	DzTupleArgument.h \
	DzTypeName.h \
	nodes/EmptyArray.h \
	EntryPoint.h \
	nodes/ExportedFunction.h \
	nodes/Function.h \
	FunctionAttribute.h \
	FunctionNotFoundException.h \
	FunctionTypeName.h \
	IPrototypeProvider.h \
	IRBuilderEx.h \
	ITypeName.h \
	IndexIterator.h \
	nodes/IndexSink.h \
	Indexed.h \
	InteropHelper.h \
	IteratorStorage.h \
	nodes/Junction.h \
	KaleidoscopeJIT.h \
	nodes/LazyEvaluation.h \
	nodes/LazySink.h \
	Node.h \
	nodes/ReferenceSink.h \
	Stack.h \
	nodes/StackSegment.h \
	nodes/TaintedSink.h \
	nodes/Terminator.h \
	Tests.h \
	UndeclaredIdentifierException.h \
	UnknownTypeException.h \
	Utility.h \
	ValueHelper.h \
	VisitorV4.h \
	VoidIterator.h \
	WithPrototypeProvider.h \
	ZipIterator.h \
	antlr4-runtime/dzBaseVisitor.h \
	antlr4-runtime/dzLexer.h \
	antlr4-runtime/dzParser.h \
	antlr4-runtime/dzVisitor.h \
	nodes/Namespace.h \
	types/AnyType.h \
	types/ArrayType.h \
	types/BooleanType.h \
	types/BuiltinType.h \
	types/ByteType.h \
	types/FunctionType.h \
	types/IPrototype.h \
	types/Int32Type.h \
	types/Int64Type.h \
	types/IteratorType.h \
	types/Prototype.h \
	types/PrototypeField.h \
	types/PrototypeFieldEmbryo.h \
	types/StringType.h \
	Type.h \
	types/TupleType.h \
	types/Uint32Type.h \
	types/UserType.h \
	types/UserTypeField.h \
	types/VoidType.h \
	types/WithPrototype.h \
	types/WithoutType.h \
	values/ArrayValue.h \
	values/ArrayValueGenerator.h \
	values/BaseValue.h \
	values/DependentValue.h \
	values/ExpandableValue.h \
	values/FunctionValue.h \
	values/IIteratable.h \
	values/ILazyValueGenerator.h \
	values/IndexedValue.h \
	values/IteratorValue.h \
	values/IteratorValueGenerator.h \
	values/LazyValue.h \
	values/NamedValue.h \
	values/ReferenceValue.h \
	values/TaintedValue.h \
	values/TupleValue.h \
	values/TypedValue.h \
	values/UserTypeValue.h \
	values/WithoutValue.h \
	wobjectdefs.h \
	wobjectimpl.h

OTHER_FILES += dz.g4 \
        main.dz \
	foo.dz

#QMAKE_CXXFLAGS += -std=c++20

LIBS += \
#    -L/home/znurre/Program/Develop/C++/llvm-project/build/lib/ \
#    -lLLVM-15git \
    -lLLVM-13 \
    -lantlr4-runtime

DEPENDPATH += /usr/include/antlr4-runtime # \
    #/home/znurre/Program/Develop/C++/llvm-project/llvm/include/ \
    #/home/znurre/Program/Develop/C++/llvm-project/build/include/
INCLUDEPATH += /usr/include/antlr4-runtime # \
    #/home/znurre/Program/Develop/C++/llvm-project/llvm/include/ \
    #/home/znurre/Program/Develop/C++/llvm-project/build/include/
