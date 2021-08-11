#ifndef KASK_H
#define KASK_H

#include "antlr4-runtime/dzBaseVisitor.h"
#include "antlr4-runtime/dzLexer.h"
#include "antlr4-runtime/dzParser.h"

#include "KaleidoscopeJIT.h"
#include "VisitorV4.h"

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
				export int main()
				{
					return 2 + 1;
				}
			)");

			QCOMPARE(result, 3);
		}

		void scenario2()
		{
			auto result = exec(R"(
				function foo()
				{
					return 1;
				}

				export int main()
				{
					return foo();
				}
			)");

			QCOMPARE(result, 1);
		}

		void scenario3()
		{
			auto result = exec(R"(
				function addFive(int v)
				{
					return v + 5;
				}

				export int main()
				{
					return addFive(1) + addFive(2);
				}
			)");

			QCOMPARE(result, 13);
		}

		void scenario4()
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

		void scenario5()
		{
			auto result = exec(R"(
				export int main()
				{
					return 1;
				}
			)");

			QCOMPARE(result, 1);
		}

		void scenario6()
		{
			auto result = exec(R"(
				function producer()
				{
					return 2;
				}

				function consumer(int value)
				{
					return value + 1;
				}

				export int main()
				{
					return consumer(producer());
				}
			)");

			QCOMPARE(result, 3);
		}

		void scenario7()
		{
			auto result = exec(R"(
				function fn(int v)
				{
					return v;
				}

				function consumer(int v)
				{
					return fn(v);
				}

				function producer()
				{
					return 2;
				}

				export int main()
				{
					return consumer(producer());
				}
			)");

			QCOMPARE(result, 2);
		}

		void scenario8()
		{
			auto result = exec(R"(
				function bar(int v)
				{
					return v;
				}

				function foo(int v)
				{
					return bar(v + 2) + bar(v + 3);
				}

				function f1()
				{
					return 2;
				}

				function f2(int v)
				{
					return v * 4;
				}

				export int main()
				{
					return f2(foo(f1() + 2));
				}
			)");

			QCOMPARE(result, 52);
		}

		void scenario9()
		{
			auto result = exec(R"(
				function func(long v)
				{
					return 3;
				}

				function func(int v, int f)
				{
					return 2;
				}

				function func(int v)
				{
					return 1;
				}

				export int main()
				{
					return func(1);
				}
			)");

			QCOMPARE(result, 1);
		}

		void scenario10()
		{
			auto result = exec(R"(
				function func(long v)
				{
					return 3;
				}

				function func(int v, int f)
				{
					return 2;
				}

				function func(int v)
				{
					return 1;
				}

				export int main()
				{
					return func(1) + 1;
				}
			)");

			QCOMPARE(result, 2);
		}

		void scenario11()
		{
			auto result = exec(R"(
				function sign(int v)
				{
					if (v < 0)
					{
						return -1;
					}

					return 1;
				}

				export int main()
				{
					return sign(3);
				}
			)");

			QCOMPARE(result, 1);
		}

		W_SLOT(scenario1)
		W_SLOT(scenario2)
		W_SLOT(scenario3)
		W_SLOT(scenario4)
		W_SLOT(scenario5)
		W_SLOT(scenario6)
		W_SLOT(scenario7)
		W_SLOT(scenario8)
		W_SLOT(scenario9)
		W_SLOT(scenario10)
		W_SLOT(scenario11)

	private:
		int exec(std::string source)
		{
			std::stringstream stream(source);

			antlr4::ANTLRInputStream input(stream);
			dzLexer lexer(&input);
			antlr4::CommonTokenStream tokens(&lexer);
			dzParser parser(&tokens);

			auto program = parser.program();

			VisitorV4 visitor(nullptr);

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
