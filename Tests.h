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

		void scenario23()
		{
			auto result = compile(R"(
				function foo(long renderer)
				{
					return 2;
				}

				function mainLoop(int init, long renderer)
				{
					return mainLoop(foo(renderer)
						, renderer
						);
				}

				export int main()
				{
					return mainLoop(0, 1L);
				}
			)");

			QVERIFY(result);
		}

		void scenario24()
		{
			auto result = exec(R"(
				function foo()
				{
					return 1;
				}

				export int main()
				{
					if (foo() != 0)
					{
						return 1;
					}

					return 0;
				}
			)");

			QCOMPARE(result, 1);
		}

		void scenario25()
		{
			auto result = exec(R"(
				function bar()
				{
					return 2;
				}

				function foo()
				{
					return 1;
				}

				export int main()
				{
					if (foo() == 0)
					{
						return 1;
					}

					bar();

					return 2;
				}
			)");

			QCOMPARE(result, 2);
		}

		void scenario26()
		{
			auto result = exec(R"(
				export int main()
				{
					return 0xFF;
				}
			)");

			QCOMPARE(result, 255);
		}

		void scenario27()
		{
			auto result = exec(R"(
				global Constant1: 12;
				global Constant2: 22u;

				function f(int v)
				{
					return 3;
				}

				function f(uint v)
				{
					return 2;
				}

				export int main()
				{
					return f(Constant1) * f(Constant2);
				}
			)");

			QCOMPARE(result, 6);
		}

		void scenario28()
		{
			auto result = exec(R"(
				function conditional1(int v)
				{
					if (v > 0)
					{
						return 1;
					}

					return 0;
				}

				function conditional2(int v)
				{
					if (conditional1(v * 2) > 0)
					{
						return 32;
					}

					return 0;
				}

				export int main()
				{
					return conditional2(10);
				}
			)");

			QCOMPARE(result, 32);
		}

		void scenario29()
		{
			auto result = exec(R"(
				struct MyStruct
				{
					x: 0,
					y: 0
				};

				function createStruct()
				{
					return MyStruct
					{
						x: 10,
						y: 20
					};
				}

				function doStuff(MyStruct s)
				{
					return s.x - s.y;
				}

				export int main()
				{
					return doStuff(createStruct());
				}
			)");

			QCOMPARE(result, -10);
		}

		void scenario30()
		{
			auto result = exec(R"(
				struct MyStruct
				{
					x: 0,
					y: 0
				};

				function createStruct()
				{
					return MyStruct
					{
						y: 20,
						x: 10
					};
				}

				function doStuff(MyStruct s)
				{
					return s.x - s.y;
				}

				export int main()
				{
					return doStuff(createStruct());
				}
			)");

			QCOMPARE(result, -10);
		}

		void scenario31()
		{
			auto result = exec(R"(
				// This is a comment
				export int main()
				{
					// This is also a comment
					return 1; // And this
				}
			)");

			QCOMPARE(result, 1);
		}

		void scenario32()
		{
			auto result = exec(R"(
				struct MyStruct
				{
					x: 0,
					y: 0
				};

				function foo()
				{
					return MyStruct
					{
						x: 1,
						y: 2
					};
				}

				function mutate(MyStruct s)
				{
					return s with
					{
						y: s.x + 10
					};
				}

				function add(MyStruct s)
				{
					return s.x + s.y;
				}

				export int main()
				{
					return add(mutate(foo()));
				}
			)");

			QCOMPARE(result, 12);
		}

		void scenario33()
		{
			auto result = exec(R"(
				struct MyStruct
				{
					x: 0,
					y: 0
				};

				function foo()
				{
					return MyStruct
					{
						x: 1,
						y: 2
					};
				}

				function mutate(MyStruct s)
				{
					return s with
					{
						y: "boo"
					};
				}

				function bar(string s)
				{
					return 30;
				}

				function add(MyStruct s)
				{
					return s.x + bar(s.y);
				}

				export int main()
				{
					return add(mutate(foo()));
				}
			)");

			QCOMPARE(result, 31);
		}

		void scenario34()
		{
			auto result = exec(R"(
				function foo()
				{
					return nothing;
				}

				function bar(without v)
				{
					return 20;
				}

				export int main()
				{
					return bar(foo());
				}
			)");

			QCOMPARE(result, 20);
		}

		void scenario35()
		{
			auto result = exec(R"(
				struct Bar
				{
					x: 0
				};

				struct Foo : Bar
				{
					y: 0
				};

				function foo()
				{
					return Foo
					{
						x: 1,
						y: 2
					};
				}

				function add(Foo f)
				{
					return f.x - f.y;
				}

				export int main()
				{
					return add(foo());
				}
			)");

			QCOMPARE(result, -1);
		}

		void scenario36()
		{
			auto result = exec(R"(
				struct Bar
				{
					x: 0
				};

				struct Foo : Bar
				{
					y: 0
				};

				function foo()
				{
					return Foo
					{
						x: 1,
						y: 2
					};
				}

				function getY(Foo f)
				{
					return f.y;
				}

				function add(Bar b)
				{
					return b.x - getY(b);
				}

				export int main()
				{
					return add(foo());
				}
			)");

			QCOMPARE(result, -1);
		}

		void scenario37()
		{
			auto result = exec(R"(
				iterator function numbersBelow(int i, int number)
				{
					if (i == number)
					{
						return i;
					}

					return i -> (i + 1, number);
				}

				function sum(int value, int number)
				{
					return value + number;
				}

				function sum(int value, (int number, ...numbers))
				{
					return sum(value + number, ...numbers);
				}

				export int main()
				{
					return sum(0, numbersBelow(0, 5));
				}
			)");

			QCOMPARE(result, 15);
		}

		void scenario38()
		{
			auto result = exec(R"(
				function sum(int value, int number)
				{
					return value + number;
				}

				function sum(int value, (int number, ...numbers))
				{
					return sum(value + number, ...numbers);
				}

				export int main()
				{
					return sum(0, [1, 2, 3]);
				}
			)");

			QCOMPARE(result, 6);
		}

		void scenario39()
		{
			auto result = exec(R"(
				struct Struct1
				{
					x
				};

				struct Struct2
				{
					y
				};

				function sum(int value, Struct1 s)
				{
					return value + s.x;
				}

				function sum(int value, (Struct1 s, ...ss))
				{
					return sum(value + s.x, ...ss);
				}

				function sum(int value, Struct2 s)
				{
					return value + s.y;
				}

				function sum(int value, (Struct2 s, ...ss))
				{
					return sum(value + s.y, ...ss);
				}

				export int main()
				{
					return sum(0, [
						Struct1
						{
							x: 1
						},
						Struct2
						{
							y: 2
						}
					]);
				}
			)");

			QCOMPARE(result, 3);
		}

		void scenario40()
		{
			auto result = exec(R"(
				function createArray()
				{
					return [1, 2, 3];
				}

				function sum(int value, int number)
				{
					return value + number;
				}

				function sum(int value, (int number, ...numbers))
				{
					return sum(value + number, ...numbers);
				}

				export int main()
				{
					return sum(0, createArray());
				}
			)");

			QCOMPARE(result, 6);
		}

		void scenario41()
		{
			auto result = exec(R"(
				function foo()
				{
					return 1;
				}

				function generator()
				{
					foo();

					return 32;
				}

				function func(int value)
				{
					return value;
				}

				export int main()
				{
					return func(generator());
				}
			)");

			QCOMPARE(result, 32);
		}

		void scenario42()
		{
			auto result = exec(R"(
				function add(int value)
				{
					return value + 2;
				}

				function add((int value, ...values))
				{
					return value + 2 -> (...values);
				}

				function sum(int product, int value)
				{
					return product + value;
				}

				function sum(int product, (int value, ...values))
				{
					return sum(product + value, ...values);
				}

				export int main()
				{
					return sum(0, add([1, 2, 3]));
				}
			)");

			QCOMPARE(result, 12);
		}

		void scenario43()
		{
			auto result = exec(R"(
				struct Child
				{
					x: 100
				};

				struct Struct
				{
					child: Child {}
				};

				function createStruct()
				{
					return Struct {};
				}

				function foo(Struct s)
				{
					return bar(s.child);
				}

				function bar(Child c)
				{
					return c.x;
				}

				export int main()
				{
					return foo(createStruct());
				}
			)");

			QCOMPARE(result, 100);
		}

		void scenario44()
		{
			auto result = exec(R"(
				function foo()
				{
					return 23;
				}

				export int main()
				{
					return foo([]);
				}
			)");

			QCOMPARE(result, 23);
		}

		void scenario45()
		{
			auto result = exec(R"(
				struct Foo
				{
					array: []
				};

				function createFoo()
				{
					return Foo {};
				}

				function foo(Foo f)
				{
					return bar(f.array);
				}

				function bar()
				{
					return 23;
				}

				export int main()
				{
					return foo(createFoo());
				}
			)");

			QCOMPARE(result, 23);
		}

		void scenario46()
		{
			auto result = exec(R"(
				function bar(int value)
				{
					if ((value > 10) && (value < 20))
					{
						return 1;
					}

					return 2;
				}

				export int main()
				{
					return bar(15);
				}
			)");

			QCOMPARE(result, 1);
		}

		void scenario47()
		{
			auto result = exec(R"(
				struct Item
				{
					value: 0,
					children: []
				};

				function createStructure()
				{
					return [
						Item
						{
							value: 1,
							children: [
								Item
								{
									value: 2
								},
								Item
								{
									value: 3
								}
							]
						},
						Item
						{
							value: 4,
							children: [
								Item
								{
									value: 5
								}
							]
						},
						Item
						{
							value: 6
						}
					];
				}

				function foo(int accumulator)
				{
					return accumulator;
				}

				function foo(int accumulator, Item item)
				{
					return foo(accumulator + item.value, item.children);
				}

				function foo(int accumulator, (Item item, ...values))
				{
					return foo(foo(accumulator + item.value, item.children), ...values);
				}

				export int main()
				{
					return foo(0, createStructure());
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
		W_SLOT(scenario23)
		W_SLOT(scenario24)
		W_SLOT(scenario25)
		W_SLOT(scenario26)
		W_SLOT(scenario27)
		W_SLOT(scenario28)
		W_SLOT(scenario29)
		W_SLOT(scenario30)
		W_SLOT(scenario31)
		W_SLOT(scenario32)
		W_SLOT(scenario33)
		W_SLOT(scenario34)
		W_SLOT(scenario35)
		W_SLOT(scenario36)
		W_SLOT(scenario37)
		W_SLOT(scenario38)
		W_SLOT(scenario39)
		W_SLOT(scenario40)
		W_SLOT(scenario41)
		W_SLOT(scenario42)
		W_SLOT(scenario43)
		W_SLOT(scenario44)
		W_SLOT(scenario45)
		W_SLOT(scenario46)
		W_SLOT(scenario47)

	private:
		ModuleInfo *compile(std::string source)
		{
			std::stringstream stream(source);

			antlr4::ANTLRInputStream input(stream);
			dzLexer lexer(&input);
			antlr4::CommonTokenStream tokens(&lexer);
			dzParser parser(&tokens);

			auto program = parser.program();

			VisitorV4 visitor(nullptr, nullptr);

			return visitor
				.visit(program)
				.as<ModuleInfo *>();

		}

		int exec(std::string source)
		{
			auto moduleInfo = compile(source);

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
