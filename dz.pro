QT += testlib

CONFIG += DEBUG

SOURCES += \
	AllIterator.cpp \
	BlockStackFrame.cpp \
	CompilerException.cpp \
	DefaultPrototypeProvider.cpp \
	DzArgument.cpp \
	DzArrayContinuation.cpp \
	DzArrayElement.cpp \
	DzArrayInit.cpp \
	DzAssignment.cpp \
	DzBinary.cpp \
	DzBooleanLiteral.cpp \
	DzConditional.cpp \
	DzContinuation.cpp \
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
	DzMemberAccess.cpp \
	DzReturn.cpp \
	DzStringLiteral.cpp \
	DzTerminator.cpp \
	DzTupleArgument.cpp \
	DzTypeName.cpp \
	DzValue.cpp \
	EntryPoint.cpp \
	IndexIterator.cpp \
	KaleidoscopeJIT.cpp \
	Stack.cpp \
	StackSegment.cpp \
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
	types/Int32Type.cpp \
	types/Int64Type.cpp \
	types/Prototype.cpp \
	types/PrototypeField.cpp \
	types/StringType.cpp \
	types/TupleType.cpp \
	types/Uint32Type.cpp \
	types/UserType.cpp \
	types/UserTypeField.cpp \
	types/VoidType.cpp \
	types/WithPrototype.cpp \
	types/WithoutType.cpp \
	values/DependentValue.cpp \
	values/ExpandableValue.cpp \
	values/NamedValue.cpp \
	values/TupleValue.cpp \
	values/TypedValue.cpp \
	values/IndexedValue.cpp \
	values/UserTypeValue.cpp

HEADERS += \
	AllIterator.h \
	BlockStackFrame.h \
	CompilerException.h \
	DefaultPrototypeProvider.h \
	DzArrayContinuation.h \
	DzArrayElement.h \
	DzArrayInit.h \
	DzArgument.h \
	DzAssignment.h \
	DzBaseArgument.h \
	DzBinary.h \
	DzBooleanLiteral.h \
	DzCallable.h \
	DzConditional.h \
	DzContinuation.h \
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
	IndexIterator.h \
	Indexed.h \
	KaleidoscopeJIT.h \
	Stack.h \
	StackSegment.h \
	Tests.h \
	UndeclaredIdentifierException.h \
	UnknownTypeException.h \
	Utility.h \
	VisitorV4.h \
	WithPrototypeProvider.h \
	antlr4-runtime/dzBaseVisitor.h \
	antlr4-runtime/dzLexer.h \
	antlr4-runtime/dzParser.h \
	antlr4-runtime/dzVisitor.h \
	types/BooleanType.h \
	types/BuiltinType.h \
	types/IPrototype.h \
	types/Int32Type.h \
	types/Int64Type.h \
	types/IteratorType.h \
	types/Prototype.h \
	types/PrototypeField.h \
	types/StringType.h \
	Type.h \
	types/TupleType.h \
	types/Uint32Type.h \
	types/UserType.h \
	types/UserTypeField.h \
	types/VoidType.h \
	types/WithPrototype.h \
	types/WithoutType.h \
	values/BaseValue.h \
	values/DependentValue.h \
	values/ExpandableValue.h \
	values/IndexedValue.h \
	values/NamedValue.h \
	values/TupleValue.h \
	values/TypedValue.h \
	values/UserTypeValue.h \
	wobjectdefs.h \
	wobjectimpl.h

OTHER_FILES += dz.g4 \
        main.dz \
	foo.dz

QMAKE_CXXFLAGS += -std=c++20

LIBS += \
    -L/home/znurre/Program/Develop/C++/llvm-project/build/lib/ \
#    -lLLVM-14git \
    -lLLVM-12 \
    -lantlr4-runtime

DEPENDPATH += /usr/include/antlr4-runtime
#    /home/znurre/Program/Develop/C++/llvm-project/llvm/include/ /home/znurre/Program/Develop/C++/llvm-project/build/include/
INCLUDEPATH += /usr/include/antlr4-runtime
#    /home/znurre/Program/Develop/C++/llvm-project/llvm/include/ /home/znurre/Program/Develop/C++/llvm-project/build/include/
