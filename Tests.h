#ifndef KASK_H
#define KASK_H

#include "antlr4-runtime/dzBaseVisitor.h"
#include "antlr4-runtime/dzLexer.h"
#include "antlr4-runtime/dzParser.h"

#include "VisitorV3.h"
#include "KaleidoscopeJIT.h"

#include "wobjectdefs.h"
#include "wobjectimpl.h"

#include <QObject>
#include <QTest>

class Tests : public QObject
{
	W_OBJECT(Tests)

	private:
		void scenario1()
		{
			auto result = exec(R"(
				function addFive(int v)
				{
					return v + 5;
				}

				function calc(int v)
				{
					return addFive(v) + addFive(2);
				}

				export int main()
				{
					return calc(2 + 1);
				}
			)");

			QCOMPARE(result, 15);
		}

		W_SLOT(scenario1)

	private:
		int exec(std::string source)
		{
			std::stringstream stream(source);

			antlr4::ANTLRInputStream input(stream);
			dzLexer lexer(&input);
			antlr4::CommonTokenStream tokens(&lexer);
			dzParser parser(&tokens);

			auto program = parser.program();

			VisitorV3 visitor;

			auto moduleInfo = visitor
				.visit(program)
				.as<ModuleInfo *>();

			auto threadSafeModule = llvm::orc::ThreadSafeModule(
				std::move(moduleInfo->module()),
				std::move(moduleInfo->context())
				);

			auto &jit = *KaleidoscopeJIT::create();
			auto error = jit->addModule(std::move(threadSafeModule));

			if (error)
			{
				return -1;
			}

			auto &mainSymbol = *jit->lookup("main");

			auto main = (int(*)())mainSymbol.getAddress();

			return main();
		}
};

W_OBJECT_IMPL(Tests)

#endif // KASK_H
