SOURCES += \
	main.cpp \
	antlr4-runtime/dzBaseVisitor.cpp \
	antlr4-runtime/dzLexer.cpp \
	antlr4-runtime/dzParser.cpp \
	antlr4-runtime/dzVisitor.cpp

HEADERS += \
	antlr4-runtime/dzBaseVisitor.h \
	antlr4-runtime/dzLexer.h \
	antlr4-runtime/dzParser.h \
	antlr4-runtime/dzVisitor.h

OTHER_FILES += dz.g4 \
	main.dz

LIBS += -lLLVM-11 -lantlr4-runtime

DEPENDPATH += /usr/include/antlr4-runtime
INCLUDEPATH += /usr/include/antlr4-runtime
