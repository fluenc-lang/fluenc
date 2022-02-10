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
	DzArrayInit.cpp \
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
	DzGlobal.cpp \
	DzGlobalTerminator.cpp \
	DzImportedFunction.cpp \
	DzInstantiation.cpp \
	DzIntegralLiteral.cpp \
	DzIteratorFunction.cpp \
	DzMemberAccess.cpp \
	DzReturn.cpp \
	DzStringLiteral.cpp \
	DzTerminator.cpp \
	DzTupleArgument.cpp \
	DzTypeName.cpp \
	DzValue.cpp \
	EntryPoint.cpp \
	IRBuilderEx.cpp \
	IndexIterator.cpp \
	InteropHelper.cpp \
	Junction.cpp \
	LazyEvaluation.cpp \
	LazySink.cpp \
	Stack.cpp \
	StackSegment.cpp \
	TaintedSink.cpp \
	UndeclaredIdentifierException.cpp \
	UnknownTypeException.cpp \
	Utility.cpp \
	VisitorV1.cpp \
	VisitorV4.cpp \
	WithPrototypeProvider.cpp \
	main.cpp \
	antlr4-runtime/dzBaseVisitor.cpp \
	antlr4-runtime/dzLexer.cpp \
	antlr4-runtime/dzParser.cpp \
	antlr4-runtime/dzVisitor.cpp \
	types/BooleanType.cpp \
	types/ByteType.cpp \
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
	values/DependentValue.cpp \
	values/ExpandableValue.cpp \
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
	DzArrayInit.h \
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
	DzGlobal.h \
	DzGlobalTerminator.h \
	DzImportedFunction.h \
	DzInstantiation.h \
	DzIntegralLiteral.h \
	DzIteratorFunction.h \
	DzMemberAccess.h \
	DzReturn.h \
	DzStringLiteral.h \
	DzTerminator.h \
	DzTupleArgument.h \
	DzTypeName.h \
	DzValue.h \
	EntryPoint.h \
	FunctionAttribute.h \
	IPrototypeProvider.h \
	IRBuilderEx.h \
	IndexIterator.h \
	Indexed.h \
	InteropHelper.h \
	Junction.h \
	KaleidoscopeJIT.h \
	LazyEvaluation.h \
	LazySink.h \
	Stack.h \
	StackSegment.h \
	TaintedSink.h \
	Tests.h \
	UndeclaredIdentifierException.h \
	UnknownTypeException.h \
	Utility.h \
	VisitorV4.h \
	VoidIterator.h \
	WithPrototypeProvider.h \
	ZipIterator.h \
	antlr4-runtime/dzBaseVisitor.h \
	antlr4-runtime/dzLexer.h \
	antlr4-runtime/dzParser.h \
	antlr4-runtime/dzVisitor.h \
	types/BooleanType.h \
	types/BuiltinType.h \
	types/ByteType.h \
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
	values/BaseValue.h \
	values/DependentValue.h \
	values/ExpandableValue.h \
	values/IndexedValue.h \
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
    -L/home/znurre/Program/Develop/C++/llvm-project/build/lib/ \
#    -lLLVM-14git \
    -lLLVM-13 \
    -lantlr4-runtime

DEPENDPATH += /usr/include/antlr4-runtime
#    /home/znurre/Program/Develop/C++/llvm-project/llvm/include/ /home/znurre/Program/Develop/C++/llvm-project/build/include/
INCLUDEPATH += /usr/include/antlr4-runtime
#    /home/znurre/Program/Develop/C++/llvm-project/llvm/include/ /home/znurre/Program/Develop/C++/llvm-project/build/include/
