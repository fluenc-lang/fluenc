QT += testlib

CONFIG += DEBUG

SOURCES += \
	AllIterator.cpp \
	CompilerException.cpp \
	DzArgument.cpp \
	DzBinary.cpp \
	DzConstant.cpp \
	DzExportedFunction.cpp \
	DzExportedFunctionTerminator.cpp \
	DzFunction.cpp \
	DzFunctionCall.cpp \
	DzFunctionTerminator.cpp \
	DzMemberAccess.cpp \
	DzTypeName.cpp \
	EntryPoint.cpp \
	KaleidoscopeJIT.cpp \
	Stack.cpp \
	UndeclaredIdentifierException.cpp \
	UnknownTypeException.cpp \
	VisitorV1.cpp \
	VisitorV4.cpp \
	main.cpp \
	antlr4-runtime/dzBaseVisitor.cpp \
	antlr4-runtime/dzLexer.cpp \
	antlr4-runtime/dzParser.cpp \
	antlr4-runtime/dzVisitor.cpp

HEADERS += \
	AllIterator.h \
	CompilerException.h \
	DzArgument.h \
	DzBinary.h \
	DzCallable.h \
	DzConstant.h \
	DzExportedFunction.h \
	DzExportedFunctionTerminator.h \
	DzFunction.h \
	DzFunctionCall.h \
	DzFunctionTerminator.h \
	DzMemberAccess.h \
	DzTypeName.h \
	DzValue.h \
	EntryPoint.h \
	FunctionAttribute.h \
	KaleidoscopeJIT.h \
	Stack.h \
	Tests.h \
	UndeclaredIdentifierException.h \
	UnknownTypeException.h \
	Utility.h \
	VisitorV4.h \
	antlr4-runtime/dzBaseVisitor.h \
	antlr4-runtime/dzLexer.h \
	antlr4-runtime/dzParser.h \
	antlr4-runtime/dzVisitor.h \
	wobjectdefs.h \
	wobjectimpl.h

OTHER_FILES += dz.g4 \
        main.dz \
	foo.dz

QMAKE_CXXFLAGS += -std=c++20

LIBS += -lLLVM-12 -lantlr4-runtime

DEPENDPATH += /usr/include/antlr4-runtime
INCLUDEPATH += /usr/include/antlr4-runtime
