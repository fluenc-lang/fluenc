QT += testlib

CONFIG += DEBUG

SOURCES += \
	AllIterator.cpp \
	CompilerException.cpp \
	DzArgument.cpp \
	DzAssignment.cpp \
	DzBinary.cpp \
	DzBooleanLiteral.cpp \
	DzConditional.cpp \
	DzExportedFunction.cpp \
	DzExportedFunctionTerminator.cpp \
	DzFunction.cpp \
	DzFunctionCall.cpp \
	DzImportedFunction.cpp \
	DzInstantiation.cpp \
	DzIntegralLiteral.cpp \
	DzMemberAccess.cpp \
	DzReturn.cpp \
	DzStringLiteral.cpp \
	DzStruct.cpp \
	DzTerminator.cpp \
	DzTypeName.cpp \
	EntryPoint.cpp \
	IndexIterator.cpp \
	KaleidoscopeJIT.cpp \
	Stack.cpp \
	TypedValue.cpp \
	UndeclaredIdentifierException.cpp \
	UnknownTypeException.cpp \
	Utility.cpp \
	VisitorV1.cpp \
	VisitorV4.cpp \
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
	types/UserType.cpp \
	types/UserTypeField.cpp \
	types/VoidType.cpp

HEADERS += \
	AllIterator.h \
	CompilerException.h \
	DzArgument.h \
	DzAssignment.h \
	DzBinary.h \
	DzBooleanLiteral.h \
	DzCallable.h \
	DzConditional.h \
	DzExportedFunction.h \
	DzExportedFunctionTerminator.h \
	DzFunction.h \
	DzFunctionCall.h \
	DzImportedFunction.h \
	DzInstantiation.h \
	DzIntegralLiteral.h \
	DzMemberAccess.h \
	DzReturn.h \
	DzStringLiteral.h \
	DzStruct.h \
	DzTerminator.h \
	DzTypeName.h \
	DzValue.h \
	EntryPoint.h \
	FunctionAttribute.h \
	IndexIterator.h \
	KaleidoscopeJIT.h \
	Stack.h \
	Tests.h \
	TypedValue.h \
	UndeclaredIdentifierException.h \
	UnknownTypeException.h \
	Utility.h \
	VisitorV4.h \
	antlr4-runtime/dzBaseVisitor.h \
	antlr4-runtime/dzLexer.h \
	antlr4-runtime/dzParser.h \
	antlr4-runtime/dzVisitor.h \
	types/BooleanType.h \
	types/BuiltinType.h \
	types/Int32Type.h \
	types/Int64Type.h \
	types/Prototype.h \
	types/PrototypeField.h \
	types/StringType.h \
	Type.h \
	types/UserType.h \
	types/UserTypeField.h \
	types/VoidType.h \
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
