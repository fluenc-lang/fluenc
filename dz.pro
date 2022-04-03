QT += testlib

CONFIG += DEBUG c++17

SOURCES += \
	AllIterator.cpp \
	ArraySink.cpp \
	BlockStackFrame.cpp \
	CompilerException.cpp \
	DefaultPrototypeProvider.cpp \
	DzArgument.cpp \
	DzArrayContinuation.cpp \
	DzArrayElement.cpp \
	DzBinary.cpp \
	DzBlockInstruction.cpp \
	DzBooleanLiteral.cpp \
	DzCharacterLiteral.cpp \
	DzConditional.cpp \
	DzContinuation.cpp \
	DzEmptyArray.cpp \
	DzExpansion.cpp \
	DzExportedFunction.cpp \
	DzExportedFunctionTerminator.cpp \
	DzFieldAccess.cpp \
	DzFunction.cpp \
	DzFunctionCall.cpp \
	DzFunctionCallProxy.cpp \
	DzGlobal.cpp \
	DzImportedFunction.cpp \
	DzInstantiation.cpp \
	DzIntegralLiteral.cpp \
	DzLocal.cpp \
	DzMemberAccess.cpp \
	DzReturn.cpp \
	DzStringLiteral.cpp \
	DzTerminator.cpp \
	DzTupleArgument.cpp \
	DzTypeName.cpp \
	DzValue.cpp \
	EntryPoint.cpp \
	FunctionNotFoundException.cpp \
	FunctionTypeName.cpp \
	IRBuilderEx.cpp \
	IndexIterator.cpp \
	IndexSink.cpp \
	InteropHelper.cpp \
	IteratorStorage.cpp \
	Junction.cpp \
	LazyEvaluation.cpp \
	LazySink.cpp \
	ReferenceSink.cpp \
	Stack.cpp \
	StackSegment.cpp \
	TaintedSink.cpp \
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
	ArraySink.h \
	BlockStackFrame.h \
	CompilerException.h \
	DefaultPrototypeProvider.h \
	DzArrayContinuation.h \
	DzArrayElement.h \
	DzArgument.h \
	DzBaseArgument.h \
	DzBinary.h \
	DzBlockInstruction.h \
	DzBooleanLiteral.h \
	DzCallable.h \
	DzCharacterLiteral.h \
	DzConditional.h \
	DzContinuation.h \
	DzEmptyArray.h \
	DzExpansion.h \
	DzExportedFunction.h \
	DzExportedFunctionTerminator.h \
	DzFieldAccess.h \
	DzFunction.h \
	DzFunctionCall.h \
	DzFunctionCallProxy.h \
	DzGlobal.h \
	DzImportedFunction.h \
	DzInstantiation.h \
	DzIntegralLiteral.h \
	DzLocal.h \
	DzMemberAccess.h \
	DzReturn.h \
	DzStringLiteral.h \
	DzTerminator.h \
	DzTupleArgument.h \
	DzTypeName.h \
	DzValue.h \
	EntryPoint.h \
	FunctionAttribute.h \
	FunctionNotFoundException.h \
	FunctionTypeName.h \
	IPrototypeProvider.h \
	IRBuilderEx.h \
	ITypeName.h \
	IndexIterator.h \
	IndexSink.h \
	Indexed.h \
	InteropHelper.h \
	IteratorStorage.h \
	Junction.h \
	KaleidoscopeJIT.h \
	LazyEvaluation.h \
	LazySink.h \
	ReferenceSink.h \
	Stack.h \
	StackSegment.h \
	TaintedSink.h \
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
