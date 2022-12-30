#include <boost/test/included/unit_test.hpp>

#include "TestHelpers.h"

#include "values/LazyValue.h"

#include "types/AnyType.h"
#include "types/Prototype.h"
#include "types/Int32Type.h"
#include "types/Int64Type.h"
#include "types/UserType.h"
#include "types/ArrayType.h"
#include "types/WithoutType.h"

#include "exceptions/MissingFieldException.h"

using namespace boost::unit_test;

BOOST_AUTO_TEST_CASE (scenario1)
{
	auto result = exec(R"(
		export i32 main()
		{
			return 2 + 1;
		}
	)");

	BOOST_TEST(result == 3);
}

BOOST_AUTO_TEST_CASE (scenario2)
{
	auto result = exec(R"(
		function foo()
		{
			return 1;
		}

		export i32 main()
		{
			return foo();
		}
	)");

	BOOST_TEST(result == 1);
}

BOOST_AUTO_TEST_CASE (scenario3)
{
	auto result = exec(R"(
		function addFive(i32 v)
		{
			return v + 5;
		}

		export i32 main()
		{
			return addFive(1) + addFive(2);
		}
	)");

	BOOST_TEST(result == 13);
}

BOOST_AUTO_TEST_CASE (scenario4)
{
	auto result = exec(R"(
		function addFive(i32 v)
		{
			return v + 5;
		}

		function calc(i32 v)
		{
			return addFive(v) + addFive(2);
		}

		export i32 main()
		{
			return calc(2 + 1);
		}
	)");

	BOOST_TEST(result == 15);
}

BOOST_AUTO_TEST_CASE (scenario5)
{
	auto result = exec(R"(
		export i32 main()
		{
			return 1;
		}
	)");

	BOOST_TEST(result == 1);
}

BOOST_AUTO_TEST_CASE (scenario6)
{
	auto result = exec(R"(
		function producer()
		{
			return 2;
		}

		function consumer(i32 value)
		{
			return value + 1;
		}

		export i32 main()
		{
			return consumer(producer());
		}
	)");

	BOOST_TEST(result == 3);
}

BOOST_AUTO_TEST_CASE (scenario7)
{
	auto result = exec(R"(
		function fn(i32 v)
		{
			return v;
		}

		function consumer(i32 v)
		{
			return fn(v);
		}

		function producer()
		{
			return 2;
		}

		export i32 main()
		{
			return consumer(producer());
		}
	)");

	BOOST_TEST(result == 2);
}

BOOST_AUTO_TEST_CASE (scenario8)
{
	auto result = exec(R"(
		function bar(i32 v)
		{
			return v;
		}

		function foo(i32 v)
		{
			return bar(v + 2) + bar(v + 3);
		}

		function f1()
		{
			return 2;
		}

		function f2(i32 v)
		{
			return v * 4;
		}

		export i32 main()
		{
			return f2(foo(f1() + 2));
		}
	)");

	BOOST_TEST(result == 52);
}

BOOST_AUTO_TEST_CASE (scenario9)
{
	auto result = exec(R"(
		function func(i64 v)
		{
			return 3;
		}

		function func(i32 v, i32 f)
		{
			return 2;
		}

		function func(i32 v)
		{
			return 1;
		}

		export i32 main()
		{
			return func(1);
		}
	)");

	BOOST_TEST(result == 1);
}

BOOST_AUTO_TEST_CASE (scenario10)
{
	auto result = exec(R"(
		function func(i64 v)
		{
			return 3;
		}

		function func(i32 v, i32 f)
		{
			return 2;
		}

		function func(i32 v)
		{
			return 1;
		}

		export i32 main()
		{
			return func(1) + 1;
		}
	)");

	BOOST_TEST(result == 2);
}

BOOST_AUTO_TEST_CASE (scenario11)
{
	auto result = exec(R"(
		function sign(i32 v)
		{
			if (v < 0)
			{
				return -1;
			}

			return 1;
		}

		export i32 main()
		{
			return sign(3);
		}
	)");

	BOOST_TEST(result == 1);
}

BOOST_AUTO_TEST_CASE (scenario12)
{
	auto result = exec(R"(
		function sign(i32 v)
		{
			if (v < 0)
			{
				return -1;
			}

			return 1;
		}

		export i32 main()
		{
			return sign(-3) * 5;
		}
	)");

	BOOST_TEST(result == -5);
}

BOOST_AUTO_TEST_CASE (scenario13)
{
	auto result = exec(R"(
		function timesFive(i32 v)
		{
			return v * 5;
		}

		function sign(i32 v)
		{
			if (v < 0)
			{
				return -1;
			}

			return 1;
		}

		export i32 main()
		{
			return timesFive(sign(-3));
		}
	)");

	BOOST_TEST(result == -5);
}

BOOST_AUTO_TEST_CASE (scenario14)
{
	auto result = exec(R"(
		function max(i32 x, i32 y)
		{
			if (x > y)
			{
				return x;
			}

			return y;
		}

		export i32 main()
		{
			return max(3, 2);
		}
	)");

	BOOST_TEST(result == 3 );
}

BOOST_AUTO_TEST_CASE (scenario15)
{
	auto result = exec(R"(
		function loop(i32 i, i32 count)
		{
			if (i < count)
			{
				return tail loop(i + 1, count);
			}

			return i;
		}

		export i32 main()
		{
			return loop(0, 5) + 2;
		}
	)");

	BOOST_TEST(result == 7);
}

BOOST_AUTO_TEST_CASE (fibonacci)
{
	auto result = exec(R"(
		function fibonacci(i32 current, i32 previous, i32 i, i32 count)
		{
			if (i < count)
			{
				return tail fibonacci(current + previous, current, i + 1, count);
			}

			return current;
		}

		export i32 main()
		{
			return fibonacci(1, 0, 0, 19);
		}
	)");

	BOOST_TEST(result == 6765);
}

BOOST_AUTO_TEST_CASE (scenario16)
{
	auto result = exec(R"(
		function consumer(i32 v)
		{
			return 1;
		}

		function consumer(i64 v)
		{
			return 2;
		}

		function producer(i32 v)
		{
			if (v > 0)
			{
				return 1;
			}

			return 2i64;
		}

		export i32 main()
		{
			return consumer(producer(1)) + consumer(producer(-1));
		}
	)");

	BOOST_TEST(result == 3);
}

BOOST_AUTO_TEST_CASE (scenario17)
{
	auto result = exec(R"(
		function isPositive(i32 v)
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

		export i32 main()
		{
			return foo(isPositive(7));
		}
	)");

	BOOST_TEST(result == 3);
}

BOOST_AUTO_TEST_CASE (scenario18)
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

		export i32 main()
		{
			return numberPlz(true) + numberPlz(false);
		}
	)");

	BOOST_TEST(result == 1);
}

BOOST_AUTO_TEST_CASE (scenario19)
{
	auto result = exec(R"(
		import i32 puts(string s);

		export i32 main()
		{
			return puts(@"foo");
		}
	)");

	BOOST_TEST(result == 4);
}

BOOST_AUTO_TEST_CASE (scenario20)
{
	auto result = exec(R"(
		struct DeStruct
		{
			num1,
			num2
		};

		function createStruct(i32 num1, i32 num2)
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

		export i32 main()
		{
			return func(createStruct(4, 6));
		}
	)");

	BOOST_TEST(result == 10);
}

BOOST_AUTO_TEST_CASE (scenario21)
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
				num: 2i64
			};
		}

		function consumer(i32 v)
		{
			return 1;
		}

		function consumer(i64 v)
		{
			return 2;
		}

		function func(DeStruct s)
		{
			return consumer(s.num);
		}

		export i32 main()
		{
			return func(createStruct());
		}
	)");

	BOOST_TEST(result == 2);
}

BOOST_AUTO_TEST_CASE (scenario22)
{
	auto result = exec(R"(
		struct DeStruct
		{
			num1: 1,
			num2: 2i64
		};

		function createStruct()
		{
			return DeStruct {};
		}

		function consumer(i32 v)
		{
			return 10;
		}

		function consumer(i64 v)
		{
			return 20;
		}

		function func(DeStruct s)
		{
			return consumer(s.num1) * consumer(s.num2);
		}

		export i32 main()
		{
			return func(createStruct());
		}
	)");

	BOOST_TEST(result == 200);
}

BOOST_AUTO_TEST_CASE (scenario23)
{
	compile(R"(
		function foo(long renderer)
		{
			return 2;
		}

		function mainLoop(i32 init, long renderer)
		{
			return mainLoop(foo(renderer)
				, renderer
				);
		}

		export i32 main()
		{
			return mainLoop(0, 1L);
		}
	)");

	BOOST_TEST(true);
}

BOOST_AUTO_TEST_CASE (scenario24)
{
	auto result = exec(R"(
		function foo()
		{
			return 1;
		}

		export i32 main()
		{
			if (foo() != 0)
			{
				return 1;
			}

			return 0;
		}
	)");

	BOOST_TEST(result == 1);
}

BOOST_AUTO_TEST_CASE (scenario25)
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

		export i32 main()
		{
			if (foo() == 0)
			{
				return 1;
			}

			bar();

			return 2;
		}
	)");

	BOOST_TEST(result == 2);
}

BOOST_AUTO_TEST_CASE (scenario26)
{
	auto result = exec(R"(
		export i32 main()
		{
			return 0xFF;
		}
	)");

	BOOST_TEST(result == 255);
}

BOOST_AUTO_TEST_CASE (scenario27)
{
	auto result = exec(R"(
		global Constant1: 12;
		global Constant2: 22u32;

		function f(i32 v)
		{
			return 3;
		}

		function f(u32 v)
		{
			return 2;
		}

		export i32 main()
		{
			return f(Constant1) * f(Constant2);
		}
	)");

	BOOST_TEST(result == 6);
}

BOOST_AUTO_TEST_CASE (scenario28)
{
	auto result = exec(R"(
		function conditional1(i32 v)
		{
			if (v > 0)
			{
				return 1;
			}

			return 0;
		}

		function conditional2(i32 v)
		{
			if (conditional1(v * 2) > 0)
			{
				return 32;
			}

			return 0;
		}

		export i32 main()
		{
			return conditional2(10);
		}
	)");

	BOOST_TEST(result == 32);
}

BOOST_AUTO_TEST_CASE (scenario29)
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

		export i32 main()
		{
			return doStuff(createStruct());
		}
	)");

	BOOST_TEST(result == -10);
}

BOOST_AUTO_TEST_CASE (scenario30)
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

		export i32 main()
		{
			return doStuff(createStruct());
		}
	)");

	BOOST_TEST(result == -10);
}

BOOST_AUTO_TEST_CASE (scenario31)
{
	auto result = exec(R"(
		// This is a comment
		export i32 main()
		{
			// This is also a comment
			return 1; // And this
		}
	)");

	BOOST_TEST(result == 1);
}

BOOST_AUTO_TEST_CASE (scenario32)
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

		export i32 main()
		{
			return add(mutate(foo()));
		}
	)");

	BOOST_TEST(result == 12);
}

BOOST_AUTO_TEST_CASE (scenario33)
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

		function bar(i32 c)
		{
			return -1;
		}

		function bar((i32 c, ...s))
		{
			return 30;
		}

		function add(MyStruct s)
		{
			return s.x + bar(s.y);
		}

		export i32 main()
		{
			return add(mutate(foo()));
		}
	)");

	BOOST_TEST(result == 31);
}

BOOST_AUTO_TEST_CASE (scenario34)
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

		export i32 main()
		{
			return bar(foo());
		}
	)");

	BOOST_TEST(result == 20);
}

BOOST_AUTO_TEST_CASE (scenario35)
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

		export i32 main()
		{
			return add(foo());
		}
	)");

	BOOST_TEST(result == -1);
}

BOOST_AUTO_TEST_CASE (scenario36)
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

		export i32 main()
		{
			return add(foo());
		}
	)");

	BOOST_TEST(result == -1);
}

BOOST_AUTO_TEST_CASE (scenario37)
{
	auto result = exec(R"(
		function numbersBelow(i32 i, i32 number)
		{
			if (i == number)
			{
				return i;
			}

			return i -> numbersBelow(i + 1, number);
		}

		function sum(i32 value, i32 number)
		{
			return value + number;
		}

		function sum(i32 value, (i32 number, ...numbers))
		{
			return tail sum(value + number, ...numbers);
		}

		export i32 main()
		{
			return sum(0, numbersBelow(0, 5));
		}
	)");

	BOOST_TEST(result == 15);
}

BOOST_AUTO_TEST_CASE (scenario38)
{
	auto result = exec(R"(
		function sum(i32 value, i32 number)
		{
			return value + number;
		}

		function sum(i32 value, (i32 number, ...numbers))
		{
			return tail sum(value + number, ...numbers);
		}

		export i32 main()
		{
			return sum(0, [1, 2, 3]);
		}
	)");

	BOOST_TEST(result == 6);
}

BOOST_AUTO_TEST_CASE (scenario39)
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

		function sum(i32 value, Struct1 s)
		{
			return value + s.x;
		}

		function sum(i32 value, (Struct1 s, ...ss))
		{
			return tail sum(value + s.x, ...ss);
		}

		function sum(i32 value, Struct2 s)
		{
			return value + s.y;
		}

		function sum(i32 value, (Struct2 s, ...ss))
		{
			return tail sum(value + s.y, ...ss);
		}

		export i32 main()
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

	BOOST_TEST(result == 3);
}

BOOST_AUTO_TEST_CASE (scenario40)
{
	auto result = exec(R"(
		function createArray()
		{
			return [1, 2, 3];
		}

		function sum(i32 value, i32 number)
		{
			return value + number;
		}

		function sum(i32 value, (i32 number, ...numbers))
		{
			return tail sum(value + number, ...numbers);
		}

		export i32 main()
		{
			return sum(0, createArray());
		}
	)");

	BOOST_TEST(result == 6);
}

BOOST_AUTO_TEST_CASE (scenario41)
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

		function func(i32 value)
		{
			return value;
		}

		export i32 main()
		{
			return func(generator());
		}
	)");

	BOOST_TEST(result == 32);
}

BOOST_AUTO_TEST_CASE (scenario42)
{
	auto result = exec(R"(
		function add(i32 value)
		{
			return value + 2;
		}

		function add((i32 value, ...values))
		{
			return value + 2 -> add(...values);
		}

		function sum(i32 product, i32 value)
		{
			return product + value;
		}

		function sum(i32 product, (i32 value, ...values))
		{
			return tail sum(product + value, ...values);
		}

		export i32 main()
		{
			return sum(0, add([1, 2, 3]));
		}
	)");

	BOOST_TEST(result == 12);
}

BOOST_AUTO_TEST_CASE (scenario43)
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

		export i32 main()
		{
			return foo(createStruct());
		}
	)");

	BOOST_TEST(result == 100);
}

BOOST_AUTO_TEST_CASE (scenario44)
{
	auto result = exec(R"(
		function foo(without item)
		{
			return 23;
		}

		export i32 main()
		{
			return foo([]);
		}
	)");

	BOOST_TEST(result == 23);
}

BOOST_AUTO_TEST_CASE (scenario45)
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

		function bar(without item)
		{
			return 23;
		}

		export i32 main()
		{
			return foo(createFoo());
		}
	)");

	BOOST_TEST(result == 23);
}

BOOST_AUTO_TEST_CASE (scenario46)
{
	auto result = exec(R"(
		function bar(i32 value)
		{
			if ((value > 10) && (value < 20))
			{
				return 1;
			}

			return 2;
		}

		export i32 main()
		{
			return bar(15);
		}
	)");

	BOOST_TEST(result == 1);
}

BOOST_AUTO_TEST_CASE (scenario47)
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

		function foo2(i32 accumulator, without item)
		{
			return accumulator;
		}

		function foo2(i32 accumulator, Item item)
		{
			return accumulator + item.value;
		}

		function foo2(i32 accumulator, (Item item, ...values))
		{
			return tail foo2(accumulator + item.value, ...values);
		}

		function foo1(i32 accumulator, without item)
		{
			return accumulator;
		}

		function foo1(i32 accumulator, Item item)
		{
			return tail foo2(accumulator + item.value, item.children);
		}

		function foo1(i32 accumulator, (Item item, ...values))
		{
			return tail foo1(foo2(accumulator + item.value, item.children), ...values);
		}

		export i32 main()
		{
			return foo1(0, createStructure());
		}
	)");

	BOOST_TEST(result == 21);
}

BOOST_AUTO_TEST_CASE (scenario48)
{
	auto result = exec(R"(
		struct Item
		{
			x: 0,
			y: 0,
			width: 0,
			height: 0,
			children: []
		};

		struct Rectangle : Item
		{
			color: 0
		};

		struct Button : Item
		{
			text
		}

		function createStructure()
		{
			return [
				Button
				{
					text: "jan",
					width: 10,
					children: [
						Rectangle
						{
							width: 20
						},
						Rectangle
						{
							width: 30,
							children: [
								Rectangle
								{
									width: 40
								}
							]
						}
					]
				}
			];
		}

		function sumWidth(i32 sum, without item)
		{
			return sum;
		}

		function sumWidth(i32 sum, Rectangle r)
		{
			return tail sumWidth(sum + r.width, r.children);
		}

		function sumWidth(i32 sum, (Rectangle r, ...items))
		{
			return tail sumWidth(sumWidth(sum + r.width, r.children), ...items);
		}

		function sumWidth(i32 sum, Button b)
		{
			return tail sumWidth(sum + b.width, b.children);
		}

		function sumWidth(i32 sum, (Button b, ...items))
		{
			return tail sumWidth(sumWidth(sum + b.width, b.children), ...items);
		}

		export i32 main()
		{
			return sumWidth(0, createStructure());
		}
	)");

	BOOST_TEST(result == 100);
}

BOOST_AUTO_TEST_CASE (scenario49)
{
	auto result = exec(R"(
		export i32 main()
		{
			return 'a';
		}
	)");

	BOOST_TEST(result == 'a');
}

BOOST_AUTO_TEST_CASE (scenario50)
{
	auto result = exec(R"(
		export i32 main()
		{
			return ' ';
		}
	)");

	BOOST_TEST(result == ' ');
}

BOOST_AUTO_TEST_CASE (scenario51)
{
	auto result = exec(R"(
		export i32 main()
		{
			return '\n';
		}
	)");

	BOOST_TEST(result == '\n');
}

BOOST_AUTO_TEST_CASE (scenario52)
{
	auto result = exec(R"(
		struct Row
		{
			x: 0
		};

		function foo(Row row)
		{
			if (row.x < 5)
			{
				return tail foo(row with { x: row.x + 1 });
			}

			return row.x;
		}

		export i32 main()
		{
			return foo(Row {});
		}
	)");

	BOOST_TEST(result == 5);
}

BOOST_AUTO_TEST_CASE (scenario53)
{
	auto result = exec(R"(
		struct Row
		{
			x: 10,
			y: 2
		};

		function foo(Row row)
		{
			return row.y;
		}

		function bar(i32 y, Row row)
		{
			return row.x * y;
		}

		function boo(Row row)
		{
			return bar(foo(row), row);
		}

		export i32 main()
		{
			return boo(Row {});
		}
	)");

	BOOST_TEST(result == 20);
}

BOOST_AUTO_TEST_CASE (scenario54)
{
	auto result = exec(R"(
		struct Item
		{
			width: 0,
			children: []
		};

		struct Button : Item
		{
			text: "foo"
		};

		struct Rectangle : Item
		{
			color: 0
		};

		function application()
		{
			return [
				Button
				{
					width: 10
				},
				Button
				{
					width: 20
				}
			];
		}

		function selectTemplate(Button button)
		{
			return Rectangle
			{
				width: button.width
			};
		}

		function drawButton(Button button)
		{
			return tail draw(selectTemplate(button));
		}

		function draw(without item)
		{
			return 0;
		}

		function draw(Rectangle rectangle)
		{
			return rectangle.width;
		}

		function draw((Rectangle rectangle, ...items))
		{
			return rectangle.width -> draw(...items);
		}

		function draw(Button button)
		{
			return drawButton(button);
		}

		function draw((Button button, ...items))
		{
			return drawButton(button) -> draw(...items);
		}

		function sum(i32 accumulator, i32 value)
		{
			return accumulator + value;
		}

		function sum(i32 accumulator, (i32 value, ...values))
		{
			return tail sum(accumulator + value, ...values);
		}

		export i32 main()
		{
			return sum(0, draw(application()));
		}
	)");

	BOOST_TEST(result == 30);
}

BOOST_AUTO_TEST_CASE (scenario55)
{
	auto result = exec(R"(
		function sum(i32 product, i32 value)
		{
			return product + value;
		}

		function sum(i32 product, (i32 value, ...values))
		{
			return tail sum(product + value, ...values);
		}

		export i32 main()
		{
			return sum(0, [1, 2, 3]);
		}
	)");

	BOOST_TEST(result == 6);
}

BOOST_AUTO_TEST_CASE (scenario56)
{
	auto result = exec(R"(
		struct Struct
		{
			x
		};

		function sum(i32 product, Struct s)
		{
			return product + s.x;
		}

		function sum(i32 product, (Struct s, ...structs))
		{
			return tail sum(product + s.x, ...structs);
		}

		function createStructs()
		{
			return [
				Struct
				{
					x: 1
				},
				Struct
				{
					x: 2
				},
				Struct
				{
					x: 3
				}
			];
		}

		export i32 main()
		{
			return sum(0, createStructs());
		}
	)");

	BOOST_TEST(result == 6);
}

BOOST_AUTO_TEST_CASE (scenario57)
{
	auto result = exec(R"(
		struct Struct
		{
			x,
			children: []
		};

		function sum(i32 product, Struct s)
		{
			return product + s.x;
		}

		function sum(i32 product, (Struct s, ...structs))
		{
			return tail sum(product + s.x, ...structs);
		}

		function createStructs()
		{
			return [
				Struct
				{
					x: 1
				},
				Struct
				{
					x: 2
				},
				Struct
				{
					x: 3
				}
			];
		}

		export i32 main()
		{
			return sum(0, createStructs());
		}
	)");

	BOOST_TEST(result == 6);
}

BOOST_AUTO_TEST_CASE (scenario58)
{
	auto result = exec(R"(
		struct Child
		{
			value
		};

		struct Struct
		{
			child1,
			child2
		};

		function createStruct()
		{
			return Struct
			{
				child1: Child
				{
					value: 1
				},
				child2: Child
				{
					value: 2
				}
			};
		}

		function foo(Struct s)
		{
			return loop(s.child1, s.child2);
		}

		function loop(Child c1, Child c2)
		{
			if ((c1.value - c2.value) == -1)
			{
				return 41;
			}

			return tail loop(c2, c1);
		}

		export i32 main()
		{
			return foo(createStruct());
		}
	)");

	BOOST_TEST(result == 41);
}

BOOST_AUTO_TEST_CASE (scenario59)
{
	auto result = exec(R"(
		function loop(i32 v1, i32 v2)
		{
			if ((v1 - v2) == -1)
			{
				return 41;
			}

			return tail loop(v2, v1);
		}

		export i32 main()
		{
			return loop(2, 1);
		}
	)");

	BOOST_TEST(result == 41);
}

BOOST_AUTO_TEST_CASE (scenario60)
{
	auto regularFunction = compileFunction(R"(
		function loop(i32 v1, i32 v2)
		{
			if ((v1 - v2) == -1)
			{
				return 41;
			}

			return loop(v2, v1);
		}
	)");

	auto iteratorFunction1 = compileFunction(R"(
		function loop(i32 v1, i32 v2)
		{
			if ((v1 - v2) == -1)
			{
				return 41;
			}

			return 10 -> loop(v2, v1);
		}
	)");

	auto iteratorFunction2 = compileFunction(R"(
		function generator(i32 i)
		{
			return i -> generator(i + 1);
		}
	)");

	auto iteratorFunction3 = compileFunction(R"(
		function generator(i32 i)
		{
			if (i < 20)
			{
				return i -> generator(i + 1);
			}

			return 20;
		}
	)");

	BOOST_TEST((regularFunction->attribute() == FunctionAttribute::None));
	BOOST_TEST((iteratorFunction1->attribute() == FunctionAttribute::Iterator));
	BOOST_TEST((iteratorFunction2->attribute() == FunctionAttribute::Iterator));
	BOOST_TEST((iteratorFunction3->attribute() == FunctionAttribute::Iterator));
}

BOOST_AUTO_TEST_CASE (scenario61)
{
	auto result = exec(R"(
		struct Struct
		{
			values: []
		};

		function sum(i32 product, i32 v)
		{
			return product + v;
		}

		function sum(i32 product, (i32 v, ...values))
		{
			return tail sum(product + v, ...values);
		}

		function add(i32 addend, i32 v)
		{
			return v + addend;
		}

		function add(i32 addend, (i32 v, ...values))
		{
			return v + addend -> add(addend, ...values);
		}

		function first(i32 v)
		{
			return v;
		}

		function first((i32 v, ...values))
		{
			return v;
		}

		function createStruct()
		{
			return Struct
			{
				values: [1, 2, 3]
			};
		}

		function foo(Struct s)
		{
			if (sum(0, s.values) < 20)
			{
				return tail foo(s with { values: add(1, s.values) });
			}

			return first(s.values);
		}

		export i32 main()
		{
			return foo(createStruct());
		}
	)");

	BOOST_TEST(result == 6);
}

BOOST_AUTO_TEST_CASE (scenario62)
{
	auto result = exec(R"(
		struct Struct
		{
			values: []
		};

		function sum(i32 product, i32 v)
		{
			return product + v;
		}

		function sum(i32 product, (i32 v, ...values))
		{
			return tail sum(product + v, ...values);
		}

		function first(i32 v)
		{
			return v;
		}

		function first((i32 v, ...values))
		{
			return v;
		}

		function createStruct()
		{
			return Struct
			{
				values: [1, 2, 3]
			};
		}

		function foo(Struct s)
		{
			if (sum(0, s.values) < 9)
			{
				return tail foo(s with { values: [2, 3, 4] });
			}

			return first(s.values);
		}

		export i32 main()
		{
			return foo(createStruct());
		}
	)");

	BOOST_TEST(result == 2);
}

BOOST_AUTO_TEST_CASE (scenario63)
{
	auto result = exec(R"(
		struct Struct
		{
			values: []
		};

		struct Element
		{
			value
		};

		function sum(i32 product, Element v)
		{
			return product + v.value;
		}

		function sum(i32 product, (Element v, ...values))
		{
			return tail sum(product + v.value, ...values);
		}

		function add(i32 addend, Element v)
		{
			return v with { value: v.value + addend };
		}

		function add(i32 addend, (Element v, ...values))
		{
			return v with { value: v.value + addend } -> add(addend, ...values);
		}

		function first(Element v)
		{
			return v.value;
		}

		function first((Element v, ...values))
		{
			return v.value;
		}

		function createStruct()
		{
			return Struct
			{
				values: [
					Element
					{
						value: 1
					},
					Element
					{
						value: 2
					},
					Element
					{
						value: 3
					}
				]
			};
		}

		function foo(Struct s)
		{
			if (sum(0, s.values) < 20)
			{
				return tail foo(s with { values: add(1, s.values) });
			}

			return first(s.values);
		}

		export i32 main()
		{
			return foo(createStruct());
		}
	)");

	BOOST_TEST(result == 6);
}

BOOST_AUTO_TEST_CASE (scenario64)
{
	auto result = exec(R"(
		struct Struct
		{
			value
		};

		struct Color
		{
			r: 0,
			g: 0,
			b: 0
		}

		function rgb(i32 r, i32 g, i32 b)
		{
			return Color
			{
				r: r,
				g: g,
				b: b
			};
		}

		function sum(Color c)
		{
			return c.r + c.g + c.b;
		}

		function sum(i32 product, Struct s)
		{
			return product + sum(s.value);
		}

		function sum(i32 product, (Struct s, ...structs))
		{
			return tail sum(product + sum(s.value), ...structs);
		}

		function createStructs()
		{
			return [
				Struct
				{
					value: rgb(1, 2, 3)
				},
				Struct
				{
					value: rgb(4, 5, 6)
				}
			];
		}

		export i32 main()
		{
			return sum(0, createStructs());
		}
	)");

	BOOST_TEST(result == 21);
}

BOOST_AUTO_TEST_CASE (scenario65)
{
	auto result = exec(R"(
		function add(i32 v1, i32 v2)
		{
			return v1 + v2;
		}

		function mul(i32 v1, i32 v2)
		{
			return v1 * v2;
		}

		function foo(i32 v1, i32 v2, function (i32, i32) fp)
		{
			return fp(v1, v2);
		}

		export i32 main()
		{
			return foo(3, 4, add) + foo(3, 4, mul);
		}
	)");

	BOOST_TEST(result == 19);
}

BOOST_AUTO_TEST_CASE (scenario66)
{
	auto result = exec(R"(
		struct Struct
		{
			fp
		};

		function add(i32 v1, i32 v2)
		{
			return v1 + v2;
		}

		function mul(i32 v1, i32 v2)
		{
			return v1 * v2;
		}

		function invoke(i32 v1, i32 v2, function (i32, i32) fp)
		{
			return fp(v1, v2);
		}

		function foo(i32 v1, i32 v2, Struct s)
		{
			return invoke(v1, v2, s.fp);
		}

		function foo(i32 v1, i32 v2, (Struct s, ...structs))
		{
			return invoke(v1, v2, s.fp) -> foo(v1, v2, ...structs);
		}

		function sum(i32 product, i32 value)
		{
			return product + value;
		}

		function sum(i32 product, (i32 value, ...values))
		{
			return tail sum(product + value, ...values);
		}

		function createStructs()
		{
			return [
				Struct
				{
					fp: add
				},
				Struct
				{
					fp: mul
				}
			];
		}

		export i32 main()
		{
			return sum(0, foo(3, 4, createStructs()));
		}
	)");

	BOOST_TEST(result == 19);
}

BOOST_AUTO_TEST_CASE (scenario67)
{
	auto result = exec(R"(
		function sum(i32 product, i32 value)
		{
			return product + value;
		}

		function sum(i32 product, (i32 value, ...values))
		{
			return tail sum(product + value, ...values);
		}

		function foo(i32 product, i32 value)
		{
			return product + value;
		}

		function foo(i32 product, (i32 value, ...values))
		{
			let result = product + value;

			return result -> foo(result, ...values);
		}

		export i32 main()
		{
			return sum(0, foo(0, [1, 2, 3]));
		}
	)");

	BOOST_TEST(result == 10);
}

BOOST_AUTO_TEST_CASE (scenario68)
{
	auto result = exec(R"(
		function add(i32 left, i32 right)
		{
			return left + right;
		}

		function add((i32 left, ...lefts), (i32 right, ...rights))
		{
			return left + right -> add(...lefts, ...rights);
		}

		function add((i32 left, ...lefts), i32 right)
		{
			return left + right;
		}

		function add(i32 left, (i32 right, ...rights))
		{
			return left + right;
		}

		function sum(i32 product, i32 value)
		{
			return product + value;
		}

		function sum(i32 product, (i32 value, ...values))
		{
			return tail sum(product + value, ...values);
		}

		export i32 main()
		{
			return sum(0, add([1, 2, 3], [4, 5, 6]));
		}
	)");

	BOOST_TEST(result == 21);
}

BOOST_AUTO_TEST_CASE (scenario69)
{
	auto result = exec(R"(
		function add(i32 left, i32 right)
		{
			return left + right;
		}

		function add((i32 left, ...lefts), (i32 right, ...rights))
		{
			return left + right -> add(...lefts, ...rights);
		}

		function add((i32 left, ...lefts), i32 right)
		{
			return left + right;
		}

		function add(i32 left, (i32 right, ...rights))
		{
			return left + right;
		}

		function sum(i32 product, i32 value)
		{
			return product + value;
		}

		function sum(i32 product, (i32 value, ...values))
		{
			return tail sum(product + value, ...values);
		}

		function addOne(i32 value)
		{
			return value + 1;
		}

		function addOne((i32 value, ...values))
		{
			return value + 1 -> addOne(...values);
		}

		export i32 main()
		{
			return sum(0, add(addOne([1, 2, 3]), addOne([4, 5, 6])));
		}
	)");

	BOOST_TEST(result == 27);
}

BOOST_AUTO_TEST_CASE (selectsTheCorrectOverload_1)
{
	auto result = exec(R"(
		struct Parent
		{
			x: 0
		}

		struct Child : Parent
		{
			y: 0
		}

		function foo(any bar)
		{
			return 1;
		}

		function foo(Parent bar)
		{
			return 2;
		}

		function foo(Child bar)
		{
			return 3;
		}

		export i32 main()
		{
			return foo(Child {});
		}
	)");

	BOOST_TEST(result == 3);
}

BOOST_AUTO_TEST_CASE (selectsTheCorrectOverload_2)
{
	auto result = exec(R"(
		struct Parent
		{
			x: 0
		}

		struct Child : Parent
		{
			y: 0
		}

		function foo(any bar)
		{
			return 1;
		}

		function foo(Parent bar)
		{
			return 2;
		}

		export i32 main()
		{
			return foo(Child {});
		}
	)");

	BOOST_TEST(result == 2);
}

BOOST_AUTO_TEST_CASE (compatibility)
{
	auto entryPoi32 = compile(R"(
		struct Unrelated
		{
			x: 0
		};

		struct Ancestor
		{
			w: 0
		}

		struct Father : Ancestor
		{
			x: 0
		}

		struct Mother : Ancestor
		{
			z: 0
		}

		struct Child : Father, Mother
		{
			y: 0
		}
	)");

	auto types = entryPoi32.types();

	auto unrelatedType = types["Unrelated"];
	auto childType = types["Child"];
	auto fatherType = types["Father"];
	auto motherType = types["Mother"];
	auto ancestorType = types["Ancestor"];

	BOOST_TEST(childType->compatibility(childType, entryPoi32) == 1);
	BOOST_TEST(childType->compatibility(fatherType, entryPoi32) == 2);
	BOOST_TEST(childType->compatibility(motherType, entryPoi32) == 2);
	BOOST_TEST(childType->compatibility(ancestorType, entryPoi32) == 3);
	BOOST_TEST(childType->compatibility(AnyType::instance(), entryPoi32) == 4);
	BOOST_TEST(childType->compatibility(unrelatedType, entryPoi32) == -1);
	BOOST_TEST(childType->compatibility(Int32Type::instance(), entryPoi32) == -1);

	BOOST_TEST(Int32Type::instance()->compatibility(Int32Type::instance(), entryPoi32) == 0);
	BOOST_TEST(Int32Type::instance()->compatibility(Int64Type::instance(), entryPoi32) == -1);
	BOOST_TEST(Int32Type::instance()->compatibility(AnyType::instance(), entryPoi32) == 1);
}

BOOST_AUTO_TEST_CASE (scenario70)
{
	auto result = exec(R"(
		function elementAt(i32 index, i32 i, i32 item)
		{
			if (i == index)
			{
				return item;
			}

			return 0;
		}

		function elementAt(i32 index, i32 i, (i32 item, ...items))
		{
			if (i == index)
			{
				return item;
			}

			return tail elementAt(index, i + 1, ...items);
		}

		function foo(i32 product, i32 index)
		{
			let table = [10, 20];

			return product + elementAt(index, 0, table);
		}

		function foo(i32 product, (i32 index, ...indexes))
		{
			let table = [10, 20];

			return tail foo(product + elementAt(index, 0, table), ...indexes);
		}

		export i32 main()
		{
			return foo(0, [1, 0, 1]);
		}
	)");

	BOOST_TEST(result == 50);
}

BOOST_AUTO_TEST_CASE (arrayType_1)
{
	auto value1 = compileValue("[1, 2, 3]");
	auto value2 = compileValue("[1, 2i64, \"foo\"]");

	BOOST_TEST(value1->type()->name() == "[i32, i32, i32]");
	BOOST_TEST(value2->type()->name() == "[i32, i64, string]");
}

BOOST_AUTO_TEST_CASE (arrayType_2)
{
	auto value1 = compileValue("[1, 2, 3]");
	auto value2 = compileValue("[1, 2i64, \"foo\"]");
	auto value3 = compileValue("[7, 5, 34]");

	BOOST_TEST(value1->type()->compatibility(value1->type(), EntryPoint()) == 0);
	BOOST_TEST(value2->type()->compatibility(value2->type(), EntryPoint()) == 0);
	BOOST_TEST(value1->type()->compatibility(value3->type(), EntryPoint()) == 0);
	BOOST_TEST(value3->type()->compatibility(value1->type(), EntryPoint()) == 0);

	BOOST_TEST(value1->type()->compatibility(value2->type(), EntryPoint()) == -1);
	BOOST_TEST(value2->type()->compatibility(value1->type(), EntryPoint()) == -1);
}

BOOST_AUTO_TEST_CASE (arrayTypePropagation1)
{
	auto entryPoi32 = compile(R"(
		function foo((i32 value, ...values))
		{
			return value -> foo(...values);
		}

		function foo(i32 value)
		{
			return value;
		}

		function bar()
		{
			return foo([1, 2]);
		}
	)");

	auto functions = entryPoi32.functions();

	auto [_1, function] = *functions.find("bar");

	Emitter emitter;

	auto functionResults = function->accept(emitter, { entryPoi32, Stack() });

	BOOST_TEST(functionResults.size() == 1);

	auto [_2, functionValues] = functionResults[0];

	BOOST_TEST(functionValues.size() == 1);

	auto lazy = functionValues.require<LazyValue>(nullptr);

	BOOST_TEST(lazy->type()->name() == "[i32, i32]");
}

BOOST_AUTO_TEST_CASE (arrayTypePropagation2)
{
	auto entryPoi32 = compile(R"(
		function foo((i32 value, ...values))
		{
			if (value > 1)
			{
				return true -> foo(...values);
			}

			return false -> foo(...values);
		}

		function foo(i32 value)
		{
			return value;
		}

		function bar()
		{
			return foo([1, 2, 3]);
		}
	)");

	auto functions = entryPoi32.functions();

	auto [_1, function] = *functions.find("bar");

	Emitter emitter;

	auto functionResults = function->accept(emitter, { entryPoi32, Stack() });

	BOOST_TEST(functionResults.size() == 1);

	auto [_2, functionValues] = functionResults[0];

	BOOST_TEST(functionValues.size() == 1);

	auto lazy = functionValues.require<LazyValue>(nullptr);

	BOOST_TEST(lazy->type()->name() == "[bool, bool, i32]");
}

BOOST_AUTO_TEST_CASE (arrayTypePropagation3)
{
	auto entryPoi32 = compile(R"(
		function foo(i32 i, i32 count)
		{
			if (i < count)
			{
				return i -> foo(i + 1, count);
			}

			return i;
		}

		function bar()
		{
			return foo(0, 3);
		}
	)");

	auto functions = entryPoi32.functions();

	auto [_1, function] = *functions.find("bar");

	Emitter emitter;

	auto functionResults = function->accept(emitter, { entryPoi32, Stack() });

	BOOST_TEST(functionResults.size() == 1);

	auto [_2, functionValues] = functionResults[0];

	BOOST_TEST(functionValues.size() == 1);

	auto lazy = functionValues.require<LazyValue>(nullptr);

	BOOST_TEST(lazy->type()->name() == "...");
}

BOOST_AUTO_TEST_CASE (arrayTypePropagation4)
{
	auto entryPoi32 = compile(R"(
		function foo((any item, ...items))
		{
			return item -> foo(...items);
		}

		function foo(any item)
		{
			return item;
		}

		function foo(without item)
		{
			return nothing;
		}

		function application()
		{
			return [1, 2, 3];
		}

		function bar()
		{
			return foo(application());
		}
	)");

	auto functions = entryPoi32.functions();

	auto [_1, function] = *functions.find("bar");

	Emitter emitter;

	auto functionResults = function->accept(emitter, { entryPoi32, Stack() });

	BOOST_TEST(functionResults.size() == 1);

	auto [_2, functionValues] = functionResults[0];

	BOOST_TEST(functionValues.size() == 1);

	auto lazy = functionValues.require<LazyValue>(nullptr);

	BOOST_TEST(lazy->type()->name() == "[i32, i32, i32]");
}

BOOST_AUTO_TEST_CASE (scenario71)
{
	auto result = exec(R"(
		function count(i32 product, i32 c)
		{
			return product + 1;
		}

		function count(i32 product, (i32 c, ...characters))
		{
			return tail count(product + 1, ...characters);
		}

		export i32 main()
		{
			return count(0, "foobar");
		}
	)");

	BOOST_TEST(result == 6);
}

BOOST_AUTO_TEST_CASE (scenario72)
{
	auto result = exec(R"(
		namespace Foo
		{
			function someFunc()
			{
				return 13 + noNamespace();
			}

			function otherFunc()
			{
				return someFunc();
			}
		}

		function noNamespace()
		{
			return 10;
		}

		export i32 main()
		{
			return Foo::otherFunc();
		}
	)");

	BOOST_TEST(result == 23);
}

BOOST_AUTO_TEST_CASE (scenario73)
{
	auto result = exec(R"(
		namespace Foo
		{
			function someFunc()
			{
				return 13 + noNamespace();
			}

			function noNamespace()
			{
				return 2;
			}

			function otherFunc()
			{
				return someFunc();
			}
		}

		function noNamespace()
		{
			return 10;
		}

		export i32 main()
		{
			return Foo::otherFunc();
		}
	)");

	BOOST_TEST(result == 15);
}

BOOST_AUTO_TEST_CASE (scenario74)
{
	auto result = exec(R"(
		namespace Foo
		{
			function someFunc()
			{
				return 13 + ::noNamespace();
			}

			function noNamespace()
			{
				return 2;
			}

			function otherFunc()
			{
				return someFunc();
			}
		}

		function noNamespace()
		{
			return 10;
		}

		export i32 main()
		{
			return Foo::otherFunc();
		}
	)");

	BOOST_TEST(result == 23);
}

BOOST_AUTO_TEST_CASE (scenario75)
{
	auto result = exec(R"(
		global MyGlobal: 16

		namespace Foo
		{
			global MyGlobal: 33

			function otherFunc()
			{
				return MyGlobal;
			}
		}

		export i32 main()
		{
			return Foo::otherFunc();
		}
	)");

	BOOST_TEST(result == 33);
}

BOOST_AUTO_TEST_CASE (scenario76)
{
	auto result = exec(R"(
		global MyGlobal: 33

		namespace Foo
		{
			function otherFunc()
			{
				return MyGlobal;
			}
		}

		export i32 main()
		{
			return Foo::otherFunc();
		}
	)");

	BOOST_TEST(result == 33);
}

BOOST_AUTO_TEST_CASE (scenario77)
{
	auto result = exec(R"(
		global MyGlobal: 16

		namespace Foo
		{
			global MyGlobal: 33

			function otherFunc()
			{
				return ::MyGlobal;
			}
		}

		export i32 main()
		{
			return Foo::otherFunc();
		}
	)");

	BOOST_TEST(result == 16);
}

BOOST_AUTO_TEST_CASE (scenario78)
{
	auto result = exec(R"(
		struct MyStruct
		{
			x: 13
		}

		namespace Foo
		{
			struct MyStruct
			{
				x: 33
			}

			function otherFunc()
			{
				return MyStruct {};
			}
		}

		function pri32X(Foo::MyStruct s)
		{
			return s.x;
		}

		export i32 main()
		{
			return pri32X(Foo::otherFunc());
		}
	)");

	BOOST_TEST(result == 33);
}

BOOST_AUTO_TEST_CASE (scenario79)
{
	auto result = exec(R"(
		struct MyStruct
		{
			x: 13
		}

		namespace Foo
		{
			function otherFunc()
			{
				return MyStruct {};
			}
		}

		function pri32X(MyStruct s)
		{
			return s.x;
		}

		export i32 main()
		{
			return pri32X(Foo::otherFunc());
		}
	)");

	BOOST_TEST(result == 13);
}

BOOST_AUTO_TEST_CASE (scenario80)
{
	auto result = exec(R"(
		struct MyStruct
		{
			x: 13
		}

		namespace Foo
		{
			struct MyStruct
			{
				x: 33
			}

			function otherFunc()
			{
				return ::MyStruct {};
			}
		}

		function pri32X(MyStruct s)
		{
			return s.x;
		}

		export i32 main()
		{
			return pri32X(Foo::otherFunc());
		}
	)");

	BOOST_TEST(result == 13);
}

BOOST_AUTO_TEST_CASE (scenario81)
{
	auto result = exec(R"(
		struct Item
		{
			children: []
		}

		struct ApplicationState
		{
			ui
		}

		function application()
		{
			return [
				Item
				{
					children: [
						Item {}
					]
				}
			];
		}

		function mainLoop(i32 count, ApplicationState state)
		{
			if (count > 0)
			{
				return 12;
			}

			return tail mainLoop(count + 1, state);
		}

		export i32 main()
		{
			let state = ApplicationState
			{
				ui: application()
			};

			return mainLoop(0, state);
		}
	)");

	BOOST_TEST(result == 12);
}

BOOST_AUTO_TEST_CASE (compatibility_3)
{
	auto result = compile(R"(
		struct Item
		{
			children: []
		}
	)");

	auto types = result.types();

	auto itemType = types["Item"];

	auto userType1 = UserType::get(itemType, { ArrayType::get({ itemType }) });
	auto userType2 = UserType::get(itemType, { WithoutType::instance() });

	// Both have the same tag
	BOOST_TEST(userType1->compatibility(userType2, result) == 1);
}

BOOST_AUTO_TEST_CASE (scenario82)
{
	auto result = exec(R"(
		struct Item
		{
			children: []
		}

		struct ApplicationState
		{
			ui
		}

		function application()
		{
			return [
				Item
				{
					children: [
						Item {},
					]
				}
			];
		}

		function update(without item)
		{
			return nothing;
		}

		function update((Item item, ...items))
		{
			return update(item) -> update(...items);
		}

		function update(Item item)
		{
			return item with
			{
				children: update(item.children),
			};
		}

		function mainLoop(i32 count, ApplicationState state)
		{
			if (count > 0)
			{
				return 12;
			}

			let as = state with
			{
				ui: update(state.ui),
			};

			return tail mainLoop(count + 1, as);
		}

		export i32 main()
		{
			let state = ApplicationState
			{
				ui: application()
			};

			return mainLoop(0, state);
		}
	)");

	BOOST_TEST(result == 12);
}

BOOST_AUTO_TEST_CASE (scenario83)
{
	auto result = exec(R"(
		struct Item
		{
			text,
			children: []
		}

		struct ApplicationState
		{
			ui
		}

		function application()
		{
			return [
				Item
				{
					text: "baz",
					children: [
						Item
						{
							text: "foo",
						},
						Item
						{
							text: "bar",
						},
					]
				}
			];
		}

		function update(without item)
		{
			return nothing;
		}

		function update((Item item, ...items))
		{
			return tail update(item) -> update(...items);
		}

		function update(Item item)
		{
			return item with
			{
				children: update(item.children),
			};
		}

		function mainLoop(i32 count, ApplicationState state)
		{
			if (count > 0)
			{
				return 12;
			}

			let as = state with
			{
				ui: update(state.ui),
			};

			return tail mainLoop(count + 1, as);
		}

		export i32 main()
		{
			let state = ApplicationState
			{
				ui: application()
			};

			return mainLoop(0, state);
		}
	)");

	BOOST_TEST(result == 12);
}

BOOST_AUTO_TEST_CASE (scenario84)
{
	BOOST_REQUIRE_THROW(exec(R"(
		struct Item;

		export i32 main()
		{
			let item = Item
			{
				does: 1,
				not: 2,
				exist: "foo",
			};

			return 12;
		}
	)")
	, MissingFieldException *
	);
}

BOOST_AUTO_TEST_CASE (scenario85)
{
	auto result = exec(R"(
		global Normal: 0;
		global MouseOver: 1;
		global Pressed: 2;

		struct ControlTemplate
		{
			normal,
			mouseOver,
			pressed
		};

		struct Item
		{
			state: 0,
			children: []
		}

		struct Rectangle : Item;
		struct Button : Item;

		function defaultTemplate(Button button)
		{
			return ControlTemplate
			{
				normal: Rectangle
				{
					children: [
						Rectangle {}
					]
				},
				mouseOver: Rectangle
				{
					children: [
						Rectangle {}
					]
				},
				pressed: Rectangle {}
			};
		}

		function selectTemplate(ControlTemplate template, Button item)
		{
			if (item.state == MouseOver)
			{
				return template.mouseOver;
			}

			if (item.state == Pressed)
			{
				return template.pressed;
			}

			return template.normal;
		}

		function draw(without item)
		{
			return nothing;
		}

		function draw(Rectangle rectangle)
		{
			return tail draw(rectangle.children);
		}

		function draw(Button button)
		{
			return tail draw(selectTemplate(defaultTemplate(button), button));
		}

		function draw((any item, ...controls))
		{
			return tail draw(...controls);
		}

		export i32 main()
		{
			let button = Button {};

			draw(selectTemplate(defaultTemplate(button), button));

			return 12;
		}
	)");

	BOOST_TEST(result == 12);
}

BOOST_AUTO_TEST_CASE (scenario86)
{
	auto result = exec(R"(
		struct Item
		{
			children: []
		}

		struct Rectangle : Item;
		struct Button : Item;

		struct ApplicationState
		{
			ui
		}

		function application()
		{
			return [
				Rectangle
				{
					children: [
						Button {},
						Button {},
					]
				}
			];
		}

		function update(ApplicationState state, without item)
		{
			return nothing;
		}

		function update(ApplicationState state, (Button item, ...items))
		{
			return item -> update(state, ...items);
		}

		function update(ApplicationState state, Button item)
		{
			return item;
		}

		function update(ApplicationState state, Rectangle item)
		{
			return item with
			{
				children: update(state, item.children),
			};
		}

		function mainLoop(i32 count, ApplicationState state)
		{
			if (count > 0)
			{
				return 12;
			}

			let as = state with
			{
				ui: update(state, state.ui),
			};

			return tail mainLoop(count + 1, as);
		}

		export i32 main()
		{
			let state = ApplicationState
			{
				ui: application()
			};

			return mainLoop(0, state);
		}
	)");

	BOOST_TEST(result == 12);
}

BOOST_AUTO_TEST_CASE (scenario87)
{
	auto result = exec(R"(
		struct Item
		{
			text
		}

		struct ApplicationState
		{
			ui
		}

		function application()
		{
			return [
				Item
				{
					text: "Foo",
				}
			];
		}

		function mainLoop(i32 count, ApplicationState state)
		{
			if (count > 0)
			{
				return 12;
			}

			return tail mainLoop(count + 1, state);
		}

		export i32 main()
		{
			let state = ApplicationState
			{
				ui: application()
			};

			return mainLoop(0, state);
		}
	)");

	BOOST_TEST(result == 12);
}

BOOST_AUTO_TEST_CASE (scenario88)
{
	auto result = exec(R"(
		struct Item
		{
			children: []
		};

		struct Button : Item
		{
			text
		};

		struct State
		{
			ui
		};

		function application()
		{
			return [
				Item
				{
					children: [
						1, 2, 3
					]
				}
			];
		}

		function update(without item, without nextItem)
		{
			return nothing;
		}

		function update((any item, ...controls), any nextItem)
		{
			return item;
		}

		function update(any item, (any nextItem, ...nextItems))
		{
			return item;
		}

		function update(Item rectangle, Item nextItem)
		{
			return rectangle with
			{
				children: update(rectangle.children, nextItem.children)
			};
		}

		function update(i32 current, i32 next)
		{
			return current;
		}

		function update((i32 current, ...currents), (i32 next, ...nexts))
		{
			return current -> update(...currents, ...nexts);
		}

		function mainLoop(i32 i, State state)
		{
			if (i > 0)
			{
				return 21;
			}

			let s = state with
			{
				ui: update(application(), application())
			};

			return tail mainLoop(i + 1, s);
		}

		export i32 main()
		{
			let state = State
			{
				ui: application()
			};

			return mainLoop(0, state);
		}
	)");

	BOOST_TEST(result == 21);
}

BOOST_AUTO_TEST_CASE (scenario89)
{
	auto result = exec(R"(
		function getBool()
		{
			return true;
		}

		export i32 main()
		{
			if (getBool())
			{
				return 1;
			}

			return 2;
		}
	)");

	BOOST_TEST(result == 1);
}

BOOST_AUTO_TEST_CASE (scenario90)
{
	auto result = exec(R"(
		function generator(i32 count, i32 i)
		{
			if (i < count)
			{
				return i -> generator(count, i + 1);
			}

			return i;
		}

		function sum(i32 product, (i32 value, ...values))
		{
			return tail sum(product + value, ...values);
		}

		function sum(i32 product, i32 value)
		{
			return product + value;
		}

		export i32 main()
		{
			let range = generator(3, 0);

			sum(0, range);

			return sum(0, range);
		}
	)");

	BOOST_TEST(result == 6);
}

BOOST_AUTO_TEST_CASE (scenario91)
{
	auto result = exec(R"(
		struct State
		{
			array
		};

		function process(i32 value)
		{
			if (value > 2)
			{
				return value;
			}

			return value * 2;
		}

		function process((i32 value, ...values))
		{
			return value -> process(...values);
		}

		function mainLoop(i32 i, State state)
		{
			let s = state with
			{
				array: process(state.array),
			};

			if (i < 1)
			{
				return tail mainLoop(i + 1, s);
			}

			return 6;
		}

		export i32 main()
		{
			let state = State
			{
				array: [1, 2]
			};

			return mainLoop(0, state);
		}
	)");

	BOOST_TEST(result == 6);
}

BOOST_AUTO_TEST_CASE (scenario92)
{
	auto result = exec(R"(
		export i32 main()
		{
			if ((0x00000101u32 & 0x00000101u32) == 0x00000101u32)
			{
				return 1;
			}

			return 0;
		}
	)");

	BOOST_TEST(result == 1);
}

BOOST_AUTO_TEST_CASE (scenario93)
{
	auto result = exec(R"(
		export i32 main()
		{
			if ((0x00010100u32 | 0x00000101u32) == 0x00010101u32)
			{
				return 1;
			}

			return 0;
		}
	)");

	BOOST_TEST(result == 1);
}

BOOST_AUTO_TEST_CASE (scenario94)
{
	auto result = exec(R"(
		export i32 main()
		{
			if ((0x00010100u32 ^ 0x00000101u32) == 0x00010001u32)
			{
				return 1;
			}

			return 0;
		}
	)");

	BOOST_TEST(result == 1);
}

BOOST_AUTO_TEST_CASE (scenario95)
{
	auto result = exec(R"(
		function add1(without previous, i32 value)
		{
			return value;
		}

		function add1(i32 previous, i32 value)
		{
			return previous + value;
		}

		function add(any previous, i32 value)
		{
			return add1(previous, value);
		}

		function add(any previous, (i32 value, ...values))
		{
			let product = add1(previous, value);

			return tail add(product, ...values);
		}

		export i32 main()
		{
			let values = [1, 2];

			return add(nothing, values);
		}
	)");

	BOOST_TEST(result == 3);
}

BOOST_AUTO_TEST_CASE (scenario96)
{
	auto result = exec(R"(
		function add1(without previous, i32 value)
		{
			return value;
		}

		function add1(i32 previous, i32 value)
		{
			return previous + value;
		}

		function add(any previous, i32 value)
		{
			return add1(previous, value);
		}

		function add(any previous, (i32 value, ...values))
		{
			let product = add1(previous, value);

			return tail add(product, ...values);
		}

		export i32 main()
		{
			let values = [1, 2, 3];

			return add(nothing, values);
		}
	)");

	BOOST_TEST(result == 6);
}

BOOST_AUTO_TEST_CASE (scenario97)
{
	auto result = exec(R"(
		struct Foo
		{
			items
		};

		function add(i32 value)
		{
			return value + 10;
		}

		function transform(i32 k, i32 value)
		{
			return value + k;
		}

		function transform(i32 k, (i32 value, ...values))
		{
			return value + k -> transform(add(value), ...values);
		}

		function sum(i32 product, (i32 value, ...values))
		{
			return tail sum(product + value, ...values);
		}

		function sum(i32 product, i32 value)
		{
			return product + value;
		}

		function mainLoop(Foo foo)
		{
			if (sum(0, foo.items) > 10)
			{
				return 32;
			}

			let mut = foo with
			{
				items: transform(0, foo.items)
			};

			return tail mainLoop(mut);
		}

		export i32 main()
		{
			let foo = Foo
			{
				items: [1, 2]
			};

			return mainLoop(foo);
		}
	)");

	BOOST_TEST(result == 32);
}

BOOST_AUTO_TEST_CASE (scenario98)
{
	auto result = exec(R"(
		function foo(f32 f)
		{
			if (f > 1.0)
			{
				return 1;
			}

			return 2;
		}

		export i32 main()
		{
			return foo(1.23 * 2.0);
		}
	)");

	BOOST_TEST(result == 1);
}

BOOST_AUTO_TEST_CASE (scenario99)
{
	auto result = exec(R"(
		struct Struct
		{
			value
		};

		export i32 main()
		{
			let s = Struct
			{
				value: 2
			};

			return s.value;
		}
	)");

	BOOST_TEST(result == 2);
}

BOOST_AUTO_TEST_CASE (scenario100)
{
	auto result = exec(R"(
		struct Struct
		{
			value
		};

		function f()
		{
			return 4;
		}

		export i32 main()
		{
			let s = Struct
			{
				value: 2
			};

			let q = s with
			{
				value: f()
			};

			return q.value;
		}
	)");

	BOOST_TEST(result == 4);
}

BOOST_AUTO_TEST_CASE (scenario101)
{
	auto result = exec(R"(
		function f(i32 value)
		{
			return value * 10;
		}

		function getNumbers(i32 i, i32 count)
		{
			if (i >= count)
			{
				return f(i);
			}

			return f(i) -> getNumbers(i + 1, count);
		}

		function sum((i32 value, ...values), i32 product)
		{
			return tail sum(...values, product + value);
		}

		function sum(i32 value, i32 product)
		{
			return product + value;
		}

		export i32 main()
		{
			let numbers = getNumbers(0, 5);

			return sum(numbers, 0);
		}
	)");

	BOOST_TEST(result == 150);
}

BOOST_AUTO_TEST_CASE (scenario102)
{
	auto result = exec(R"(
		struct Foo
		{
			children
		};

		function sum((i32 value, ...values), i32 product)
		{
			return tail sum(...values, product + value);
		}

		function sum(i32 value, i32 product)
		{
			return product + value;
		}

		function add(Foo foo, (i32 left, ...lefts))
		{
			return left -> add(foo, ...lefts);
		}

		function add(Foo foo, i32 left)
		{
			return left;
		}

		export i32 main()
		{
			let foo = Foo
			{
				children: [1, 2]
			};

			let input1 = [1, 2, 3];

			let numbers = add(foo, input1);

			return sum(numbers, 0);
		}
	)");

	BOOST_TEST(result == 6);
}

BOOST_AUTO_TEST_CASE (scenario103)
{
	auto result = exec(R"(
		struct Item
		{
			children: nothing
		};

		struct State
		{
			ui
		};

		function layout(any item, any nextItem)
		{
			return item;
		}

		function layout(Item item, Item nextItem)
		{
			return item;
		}

		function layout((Item item, ...items), any nextItem)
		{
			return item;
		}

		function layout(any item, (Item nextItem, ...nextItems))
		{
			return nextItem;
		}

		function layout((Item item, ...items), (Item nextItem, ...nextItems))
		{
			return item -> layout(...items, ...nextItems);
		}

		function solve(Item item, Item nextItem)
		{
			return item with
			{
				children: layout(item.children, nextItem.children)
			};
		}

		function update(Item item, Item nextItem)
		{
			return solve(item, nextItem);
		}

		function update((Item item, ...items), (Item nextItem, ...nextItems))
		{
			return solve(item) -> update(...items);
		}

		function application()
		{
			return [
				Item
				{
					children: [
						Item {},
						Item {}
					]
				}
			];
		}

		function foo(State state, i32 i)
		{
			if (i > 1)
			{
				return 22;
			}

			let s = state with
			{
				ui: update(state.ui, application())
			};

			return tail foo(s, i + 1);
		}

		export i32 main()
		{
			let state = State
			{
				ui: application()
			};

			return foo(state, 0);
		}
	)");

	BOOST_TEST(result == 22);
}

BOOST_AUTO_TEST_CASE (scenario104)
{
	auto result = exec(R"(
		struct State
		{
			values
		};

		function foo((any item, ...items))
		{
			return item -> foo(...items);
		}

		function foo(any item)
		{
			return item;
		}

		function foo(without item)
		{
			return nothing;
		}

		function bar(i32 count, State s)
		{
			if (count > 0)
			{
				return 22;
			}

			return tail bar(count + 1, s);
		}

		export i32 main()
		{
			let state = State
			{
				values: foo(foo([1, 2, 3]))
			};

			return bar(0, state);
		}
	)");

	BOOST_TEST(result == 22);
}

BOOST_AUTO_TEST_CASE (scenario105)
{
	auto result = exec(R"(
		struct State
		{
			values
		};

		function array()
		{
			return [1, 2, 3];
		}

		function foo((any item, ...items))
		{
			return item -> foo(...items);
		}

		function foo(any item)
		{
			return item;
		}

		function bar(i32 count, State s)
		{
			if (count > 0)
			{
				return 22;
			}

			return tail bar(count + 1, s);
		}

		export i32 main()
		{
			let state = State
			{
				values: foo(array())
			};

			return bar(0, state);
		}
	)");

	BOOST_TEST(result == 22);
}

BOOST_AUTO_TEST_CASE (scenario106)
{
	auto result = exec(R"(
		struct State
		{
			values
		};

		function array()
		{
			return [1, 2, 3];
		}

		function foo((any item, ...items))
		{
			return item -> foo(...items);
		}

		function foo(any item)
		{
			return item;
		}

		function bar(i32 count, State s)
		{
			if (count > 0)
			{
				return 22;
			}

			return tail bar(count + 1, s);
		}

		function boo(function() a)
		{
			let state = State
			{
				values: foo(a())
			};

			return bar(0, state);
		}

		export i32 main()
		{
			return boo(array);
		}
	)");

	BOOST_TEST(result == 22);
}

BOOST_AUTO_TEST_CASE (scenario107)
{
	auto result = exec(R"(
		struct State
		{
			values
		};

		struct Color
		{
			r: 0,
			g: 0,
			b: 0
		};

		struct Rectangle
		{
			color: Color {}
		};

		function foo((any item, ...items))
		{
			return item -> foo(...items);
		}

		function foo(any item)
		{
			return item;
		}

		function bar(i32 count, State s)
		{
			if (count >= 1)
			{
				return 45;
			}

			return tail bar(count + 1, s);
		}

		export i32 main()
		{
			let state = State
			{
				values: foo([
					Rectangle {},
					Rectangle {}
				])
			};

			return bar(0, state);
		}
	)");

	BOOST_TEST(result == 45);
}

BOOST_AUTO_TEST_CASE (scenario108)
{
	auto result = exec(R"(
		struct Callbacks
		{
			callback
		};

		struct State
		{
			values
		};

		struct Color
		{
			r: 0,
			g: 0,
			b: 0
		};

		struct Rectangle
		{
			color: Color {}
		};

		function foo((any item, ...items))
		{
			return item -> foo(...items);
		}

		function foo(any item)
		{
			return item;
		}

		function bar(i32 count, State s)
		{
			if (count >= 1)
			{
				return 45;
			}

			return tail bar(count + 1, s);
		}

		function array()
		{
			return [
				Rectangle {},
				Rectangle {}
			];
		}

		function invoke(function() c)
		{
			return c();
		}

		function boo(Callbacks c)
		{
			let state = State
			{
				values: foo(invoke(c.callback))
			};

			return bar(0, state);
		}

		export i32 main()
		{
			let callbacks = Callbacks
			{
				callback: array
			};

			return boo(callbacks);
		}
	)");

	BOOST_TEST(result == 45);
}

BOOST_AUTO_TEST_CASE (scenario109)
{
	auto result = exec(R"(
		struct State
		{
			values
		};

		struct Color
		{
			r: 0,
			g: 0,
			b: 0
		};

		struct Rectangle
		{
			color: Color {}
		};

		function getRed(Color color)
		{
			return color.r;
		}

		function foo((Rectangle item, ...items))
		{
			return getRed(item.color) -> foo(...items);
		}

		function foo(Rectangle item)
		{
			return getRed(item.color);
		}

		function bar(i32 count, State s)
		{
			if (count >= 1)
			{
				return 45;
			}

			return tail bar(count + 1, s);
		}

		export i32 main()
		{
			let state = State
			{
				values: foo([
					Rectangle {},
					Rectangle {}
				])
			};

			return bar(0, state);
		}
	)");

	BOOST_TEST(result == 45);
}

BOOST_AUTO_TEST_CASE (scenario110)
{
	auto result = compile(R"(
		function loop()
		{
			return tail loop();
		}

		export i32 main()
		{
			return loop();
		}
	)");

	Emitter emitter;

	for (auto &root : result.roots())
	{
		root->accept(emitter, { result, Stack() });
	}

	auto module = result.module();

	BOOST_TEST(module->getFunction("main"));
}

BOOST_AUTO_TEST_CASE (scenario111)
{
	auto result = compile(R"(
		struct Stage;
		struct Water : Stage;
		struct Moon : Stage;

		global water: Water {};
		global moon: Moon {};
	)");

	Emitter emitter;

	std::vector<const Type *> types;

	for (auto &[name, global] : result.globals())
	{
		for (auto &[ep, values] : global->accept(emitter, { result, Stack() }))
		{
			for (auto &value : values)
			{
				auto type = value->type();

				types.push_back(type);
			}
		}
	}

	BOOST_TEST(types.size() == 2);

	auto type1 = types[0];
	auto type2 = types[1];

	BOOST_TEST(type1->compatibility(type2, result) == -1);
	BOOST_TEST(type1->compatibility(type1, result) == 0);
	BOOST_TEST(type2->compatibility(type2, result) == 0);
}

BOOST_AUTO_TEST_CASE (scenario112)
{
	auto result = compile(R"(
		struct State
		{
			callback
		}

		function func1()
		{
			return 1;
		}

		function func2()
		{
			return 2;
		}

		global instance1: State
		{
			callback: func1
		}

		global instance2: State
		{
			callback: func2
		}
	)");

	Emitter emitter;

	std::vector<const Type *> types;

	for (auto &[name, global] : result.globals())
	{
		for (auto &[ep, values] : global->accept(emitter, { result, Stack() }))
		{
			for (auto &value : values)
			{
				auto type = value->type();

				types.push_back(type);
			}
		}
	}

	BOOST_TEST(types.size() == 2);

	auto type1 = types[0];
	auto type2 = types[1];

	BOOST_TEST(type1->compatibility(type2, result) == 1);
	BOOST_TEST(type1->compatibility(type1, result) == 0);
	BOOST_TEST(type2->compatibility(type2, result) == 0);
}

BOOST_AUTO_TEST_CASE (scenario113)
{
	auto result = exec(R"(
		function generator(i32 i, i32 count)
		{
			if (i < count)
			{
				return i -> generator(i + 1, count);
			}

			return i;
		}

		function add(without v1, i32 v2)
		{
			return v2;
		}

		function add(i32 v1, i32 v2)
		{
			return v1 + v2;
		}

		function foo(any accumulated, (i32 value, ...values))
		{
			let added = add(accumulated, value);

			return tail foo(added, ...values);
		}

		function foo(any accumulated, i32 value)
		{
			return add(accumulated, value);
		}

		export i32 main()
		{
			return foo(nothing, generator(0, 3));
		}
	)");

	BOOST_TEST(result == 6);
}

BOOST_AUTO_TEST_CASE (scenario114)
{
	auto result = exec(R"(
		export i32 main()
		{
			let predicate = 1 > 2;

			if (!predicate)
			{
				return 1;
			}

			return 2;
		}
	)");

	BOOST_TEST(result == 1);
}

BOOST_AUTO_TEST_CASE (scenario115)
{
	auto result = exec(R"(
		export i32 main()
		{
			let predicate = 1 > 2;

			if ((!predicate) || (!predicate))
			{
				return 1;
			}

			return 2;
		}
	)");

	BOOST_TEST(result == 1);
}

BOOST_AUTO_TEST_CASE (scenario116)
{
	auto result = exec(R"(
		struct State
		{
			func
		}

		function value1()
		{
			return 1;
		}

		function value2()
		{
			return 2;
		}

		function invoke(function() callback)
		{
			return callback();
		}

		function generator(i32 i, State previous)
		{
			if (i == 0)
			{
				let s = State
				{
					func: value2
				};

				return 10 -> generator(i + 1, s);
			}

			return invoke(previous.func);
		}

		function sum(i32 product, (i32 value, ...values))
		{
			return tail sum(product + value, ...values);
		}

		function sum(i32 product, i32 value)
		{
			return product + value;
		}

		export i32 main()
		{
			let init = State
			{
				func: value1
			};

			return sum(0, generator(0, init));
		}
	)");

	BOOST_TEST(result == 12);
}

BOOST_AUTO_TEST_CASE (scenario117)
{
	auto result = exec(R"(
		struct State
		{
			ui,
			user
		}

		struct User;

		struct Item
		{
			children: []
		}

		function application()
		{
			return [
				Item {},
			];
		}

		function foo((any item, ...items))
		{
			return item -> foo(...items);
		}

		function foo(any item)
		{
			return item;
		}

		function mainLoop(i32 index, State state)
		{
			if (index > 0)
			{
				return 32;
			}

			let s = state with
			{
				user: process(state, state.ui)
			};

			return tail mainLoop(index + 1, s);
		}

		function process(State state, (Item item, ...controls))
		{
			let s = state with
			{
				user: process(state, item.children)
			};

			return tail process(s, ...controls);
		}

		function process(State state, Item item)
		{
			return process(state, item.children);
		}

		function process(State state, without item)
		{
			return state.user;
		}

		export i32 main()
		{
			let state = State
			{
				ui: foo(application()),
				user: User {}
			};

			return mainLoop(0, state);
		}
	)");

	BOOST_TEST(result == 32);
}

BOOST_AUTO_TEST_CASE (scenario118)
{
	auto result = exec(R"(
		struct State
		{
			ui
		}

		struct Item1;
		struct Item2;

		function application()
		{
			return [
				Item1 {},
				Item2 {}
			];
		}

		function update((Item1 item, ...items), (any nextItem, ...nextItems))
		{
			return 1 -> update(...items, ...nextItems);
		}

		function update(Item1 item, any nextItem)
		{
			return 1;
		}

		function update((Item2 item, ...items), (any nextItem, ...nextItems))
		{
			return 2 -> update(...items, ...nextItems);
		}

		function update(Item2 item, any nextItem)
		{
			return 2;
		}

		function update((any item, ...items), any nextItem)
		{
			return 0;
		}

		function update(any item, (any nextItem, ...nextItems))
		{
			return 0;
		}

		function expand((any item, ...items))
		{
			return item -> expand(...items);
		}

		function expand(any item)
		{
			return item;
		}

		function sum(i32 product, (i32 value, ...values))
		{
			return tail sum(product + value, ...values);
		}

		function sum(i32 product, i32 value)
		{
			return product + value;
		}

		export i32 main()
		{
			let ui = application();
			let res = update(expand(ui), expand(application()));

			return sum(0, res)
		}
	)");

	BOOST_TEST(result == 3);
}

BOOST_AUTO_TEST_CASE (scenario119)
{
	auto result = exec(R"(
		struct State
		{
			ui
		}

		struct Item
		{
			children: []
		}

		struct Item1 : Item;

		struct LayoutItem
		{
			item
		}

		function application()
		{
			return [
				Item {},
				Item
				{
					children: [
						LayoutItem
						{
							item: Item1 {}
						},
						Item1 {}
					]
				},
				Item1 {}
			];
		}

		function foo(without item)
		{
			return 0;
		}

		function foo(Item1 item)
		{
			return 10;
		}

		function foo((Item item, ...items))
		{
			return foo(item) -> foo(...items);
		}

		function foo(Item item)
		{
			return sum(0, foo(item.children));
		}

		function foo((LayoutItem item, ...items))
		{
			return foo(item.item) -> foo(...items);
		}

		function sum(i32 product, (i32 value, ...values))
		{
			return tail sum(product + value, ...values);
		}

		function sum(i32 product, i32 value)
		{
			return product + value;
		}

		export i32 main()
		{
			let ui = application();
			let res = foo(ui);

			return sum(0, res)
		}
	)");

	BOOST_TEST(result == 30);
}

BOOST_AUTO_TEST_CASE (scenario120)
{
	auto result = exec(R"(
		struct Item
		{
			value
		}

		function createItem(i32 index)
		{
			return Item
			{
				value: index
			};
		}

		function item((i32 v, ...vs))
		{
			return createItem(v) -> item(...vs);
		}

		function item(i32 v)
		{
			return createItem(v);
		}

		function application()
		{
			return item([1, 2, 3]);
		}

		function sum(i32 product, (Item item, ...items))
		{
			return tail sum(product + item.value, ...items);
		}

		function sum(i32 product, Item item)
		{
			return product + item.value;
		}

		function consume((Item item, ...items))
		{
			return tail consume(...items);
		}

		function consume(Item item)
		{
			return nothing;
		}

		export i32 main()
		{
			let items = application();

			consume(items);

			return sum(0, items);
		}
	)");

	BOOST_TEST(result == 6);
}

BOOST_AUTO_TEST_CASE (scenario121)
{
	auto result = exec(R"(
		function foo(i32 counter, any value, (any nextValue, ...nextValues))
		{
			return 0;
		}

		function foo(i32 counter, (any value, ...values), any nextValue)
		{
			return 0;
		}

		function foo(i32 counter, (i32 value, ...values), (i32 nextValue, ...nextValues))
		{
			return counter + value + nextValue -> foo(counter + 1, ...values, ...nextValues);
		}

		function foo(i32 counter, i32 value, i32 nextValue)
		{
			return counter + value + nextValue;
		}

		function consume((any item, ...items))
		{
			return tail consume(...items);
		}

		function consume(any item)
		{
			return nothing;
		}

		function sum(i32 product, (i32 item, ...items))
		{
			return tail sum(product + item, ...items);
		}

		function sum(i32 product, i32 item)
		{
			return product + item;
		}

		function doStuff(i32 init)
		{
			let items1 = [1, 2];
			let items2 = [2, 3];

			let values = foo(init, items1, items2);

			consume(values);

			return sum(0, values);
		}

		export i32 main()
		{
			return doStuff(0);
		}
	)");

	BOOST_TEST(result == 9);
}

BOOST_AUTO_TEST_CASE (scenario122)
{
	auto result = exec(R"(
		function add(i32 left, i32 right)
		{
			return left + right;
		}

		function add((i32 left, ...lefts), (i32 right, ...rights))
		{
			return left + right -> add(...lefts, ...rights);
		}

		function add((i32 left, ...lefts), i32 right)
		{
			return left + right;
		}

		function add(i32 left, (i32 right, ...rights))
		{
			return left + right;
		}

		function sum(i32 product, i32 value)
		{
			return product + value;
		}

		function sum(i32 product, (i32 value, ...values))
		{
			return tail sum(product + value, ...values);
		}

		function generate1(i32 index, i32 count)
		{
			if (index >= count)
			{
				return index;
			}

			return index -> generate1(index + 1, count);
		}

		function generate2(i32 index, i32 count)
		{
			if (index >= count)
			{
				return index;
			}

			return index -> generate2(index + 1, count);
		}

		export i32 main()
		{
			return sum(0, add(generate1(1, 3), generate2(4, 6)));
		}
	)");

	BOOST_TEST(result == 21);
}

BOOST_AUTO_TEST_CASE (scenario123)
{
	auto result = exec(R"(
		export i32 foo(i32 value)
		{
			return value * 2;
		}

		export i32 main()
		{
			return foo(123);
		}
	)");

	BOOST_TEST(result == 246);
}

test_suite* init_unit_test_suite(int /*argc*/, char* /*argv*/[] )
{
	llvm::InitializeAllTargetInfos();
	llvm::InitializeAllTargets();
	llvm::InitializeAllTargetMCs();
	llvm::InitializeAllAsmParsers();
	llvm::InitializeAllAsmPrinters();

	return 0;
}
