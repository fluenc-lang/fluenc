#ifndef KASK_H
#define KASK_H

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

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

		void scenario12()
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
					return sign(-3) * 5;
				}
			)");

			QCOMPARE(result, -5);
		}

		void scenario13()
		{
			auto result = exec(R"(
				function timesFive(int v)
				{
					return v * 5;
				}

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
					return timesFive(sign(-3));
				}
			)");

			QCOMPARE(result, -5);
		}

		void scenario14()
		{
			auto result = exec(R"(
				function max(int x, int y)
				{
					if (x > y)
					{
						return x;
					}

					return y;
				}

				export int main()
				{
					return max(3, 2);
				}
			)");

			QCOMPARE(result, 3 );
		}

		void scenario15()
		{
			auto result = exec(R"(
				function loop(int i, int count)
				{
					if (i < count)
					{
						return loop(i + 1, count);
					}

					return i;
				}

				export int main()
				{
					return loop(0, 5) + 2;
				}
			)");

			QCOMPARE(result, 7);
		}

		void fibonacci()
		{
			auto result = exec(R"(
				function fibonacci(int current, int previous, int i, int count)
				{
					if (i < count)
					{
						return fibonacci(current + previous, current, i + 1, count);
					}

					return current;
				}

				export int main()
				{
					return fibonacci(1, 0, 0, 19);
				}
			)");

			QCOMPARE(result, 6765);
		}

		void scenario16()
		{
			auto result = exec(R"(
				function consumer(int v)
				{
					return 1;
				}

				function consumer(long v)
				{
					return 2;
				}

				function producer(int v)
				{
					if (v > 0)
					{
						return 1;
					}

					return 2L;
				}

				export int main()
				{
					return consumer(producer(1)) + consumer(producer(-1));
				}
			)");

			QCOMPARE(result, 3);
		}

		void scenario17()
		{
			auto result = exec(R"(
				function isPositive(int v)
				{
					return v > 0;
				}

				function foo(bool b)
				{
					if (b)
					{
						return 3;
					}

					return -2;
				}

				export int main()
				{
					return foo(isPositive(7));
				}
			)");

			QCOMPARE(result, 3);
		}

		void scenario18()
		{
			auto result = exec(R"(
				function numberPlz(bool positive)
				{
					if (positive)
					{
						return 3;
					}

					return -2;
				}

				export int main()
				{
					return numberPlz(true) + numberPlz(false);
				}
			)");

			QCOMPARE(result, 1);
		}

		void scenario19()
		{
			auto result = exec(R"(
				function func(string v)
				{
					return 1;
				}

				export int main()
				{
					return func("foo");
				}
			)");

			QCOMPARE(result, 1);
		}

		void scenario20()
		{
			auto result = exec(R"(
				struct DeStruct
				{
					num1,
					num2
				};

				function createStruct(int num1, int num2)
				{
					return DeStruct
					{
						num1: num1,
						num2: num2
					};
				}

				function func(DeStruct s)
				{
					return s.num1 + s.num2;
				}

				export int main()
				{
					return func(createStruct(4, 6));
				}
			)");

			QCOMPARE(result, 10);
		}

		void scenario21()
		{
			auto result = exec(R"(
				struct DeStruct
				{
					num
				};

				function createStruct()
				{
					return DeStruct
					{
						num: 2L
					};
				}

				function consumer(int v)
				{
					return 1;
				}

				function consumer(long v)
				{
					return 2;
				}

				function func(DeStruct s)
				{
					return consumer(s.num);
				}

				export int main()
				{
					return func(createStruct());
				}
			)");

			QCOMPARE(result, 2);
		}

		void scenario22()
		{
			auto result = exec(R"(
				struct DeStruct
				{
					num1: 1,
					num2: 2L
				};

				function createStruct()
				{
					return DeStruct {};
				}

				function consumer(int v)
				{
					return 10;
				}

				function consumer(long v)
				{
					return 20;
				}

				function func(DeStruct s)
				{
					return consumer(s.num1) * consumer(s.num2);
				}

				export int main()
				{
					return func(createStruct());
				}
			)");

			QCOMPARE(result, 200);
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
		W_SLOT(scenario12)
		W_SLOT(scenario13)
		W_SLOT(scenario14)
		W_SLOT(scenario15)
		W_SLOT(fibonacci)
		W_SLOT(scenario16)
		W_SLOT(scenario17)
		W_SLOT(scenario18)
		W_SLOT(scenario19)
		W_SLOT(scenario20)
		W_SLOT(scenario21)
		W_SLOT(scenario22)

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
