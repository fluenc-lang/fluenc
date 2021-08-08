QT += testlib

CONFIG += DEBUG c++20

SOURCES += \
	CallContext.cpp \
	CompilerException.cpp \
	DebugPrinter.cpp \
	DzBinary.cpp \
	DzBinaryNg.cpp \
	DzCallNg.cpp \
	DzCallable.cpp \
	DzClosure.cpp \
	DzClosureAccessor.cpp \
	DzConstant.cpp \
	DzConstantNg.cpp \
	DzConsumer.cpp \
	DzEntryPointNg.cpp \
	DzFunction.cpp \
	DzFunctionCall.cpp \
	DzFunctionNg.cpp \
	DzMember.cpp \
	DzMemberAccess.cpp \
	DzMemberAccessNg.cpp \
	DzMemberNg.cpp \
	DzNodeNg.cpp \
	DzParameter.cpp \
	DzReturn.cpp \
	DzReturnNg.cpp \
	DzTerminator.cpp \
	DzTypeName.cpp \
	DzTypeNameNg.cpp \
	DzValueNg.cpp \
	EntryPoint.cpp \
	EntryPointInfo.cpp \
	KaleidoscopeJIT.cpp \
	Tests.cpp \
	UndeclaredIdentifierException.cpp \
	UnknownTypeException.cpp \
	VisitorV1.cpp \
	VisitorV2.cpp \
	VisitorV3.cpp \
	VisitorV4.cpp \
	main.cpp \
	antlr4-runtime/dzBaseVisitor.cpp \
	antlr4-runtime/dzLexer.cpp \
	antlr4-runtime/dzParser.cpp \
	antlr4-runtime/dzVisitor.cpp

HEADERS += \
	CallContext.h \
	CompilerException.h \
	DebugPrinter.h \
	DzBinary.h \
	DzBinaryNg.h \
	DzCallNg.h \
	DzCallable.h \
	DzClosure.h \
	DzClosureAccessor.h \
	DzConstant.h \
	DzConstantNg.h \
	DzConsumer.h \
	DzEntryPointNg.h \
	DzFunction.h \
	DzFunctionCall.h \
	DzFunctionNg.h \
	DzMember.h \
	DzMemberAccess.h \
	DzMemberAccessNg.h \
	DzMemberNg.h \
	DzNodeNg.h \
	DzParameter.h \
	DzReturn.h \
	DzReturnNg.h \
	DzTerminator.h \
	DzTypeName.h \
	DzTypeNameNg.h \
	DzValue.h \
	DzValueNg.h \
	EntryPoint.h \
	EntryPointInfo.h \
	FunctionAttribute.h \
	KaleidoscopeJIT.h \
	Tests.h \
	UndeclaredIdentifierException.h \
	UnknownTypeException.h \
	VisitorV1.h \
	VisitorV2.h \
	VisitorV3.h \
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

LIBS += -lLLVM-12 -lantlr4-runtime

DEPENDPATH += /usr/include/antlr4-runtime
INCLUDEPATH += /usr/include/antlr4-runtime
