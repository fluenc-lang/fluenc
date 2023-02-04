# FluenC

FluenC is a C-inspired, functional-first language, with a focus on simplicity. 

It's hard to know where to start explaining what FluenC is, and how it works, since the language uses many unique features that works in symbiosis to produce the end result: A language as easy as JavaScript, and as performant as C.

For now, let's start with with some code!

## Hello world in FluenC

```js
import i32 puts(string str);

export i32 main()
{
    return puts(@"Hello world!");
}
```

## Memory management

Perhaps the biggest difference between FluenC and other languages is the fact that the memory model of FluenC is so simple. The language is designed in such a way that memory is never allocated in runtime, be it stack allocation or heap allocation. Instead, each function parameter and return value has a fixed memory address, with storage being allocated upfront at application startup, or even in the data section of the binary in the future. This makes the memory usage of a basic FluenC program very predictable.

No garbage collectors, no manual allocation or deallocation, no borrow checker. Only simplicity.

It's still possible to perform dynamic memory allocation by utilizing the C interop facilities of the language, which can be useful when extending the core libraries for example.

## Generically typed

FluenC is a generically typed language. What does this mean?

If you come from a C++ background, the easiest way of thinking about the type system in FluenC is to imagine that every single value is a template. The structs in FluenC have no types for their members by default, and might look a bit alien.

```c
struct Foo
{
    member1,
    member2
};
```

This corresponds to the following code in C++:

```cpp
template<typename T1, typename T2>
struct Foo
{
    T1 member1,
    T2 member2
};
```

Any member without an explicit type will need to be initialized when instantiating the struct type. A member can also have a default value, in which case a value does not need to be specified at the time of instantiation, and the type will be inferred from the expression.

```js
struct Foo
{
    member1: 1,
    member2: "foo"
};
```

## Pattern matching

Pattern matching is performed using function overloads in FluenC. This works in perfect symbiosis with the generic type system. The correct overload will be selected based on the actual type of the value.

```js
struct Foo
{
    value
};

function processValue(i32 value)
{
    return value;
}

function processValue(i64 value)
{
    return 42;
}

function calculate(Foo left, Foo right)
{
    return processValue(left.value) + processValue(right.value);
}

export i32 main()
{
    let value1 = Foo
    {
        value: 1 // type is inferred as int
    };
    
    let value2 = Foo
    {
        value: 2i64 // type is inferred as long
    };
    
    return calculate(value1, value2); // Will return 43
}
```

## Iterators

Since there are no allocations, consumption of arrays and similar structures works slightly different from other languages. You only ever work with one element at a time, making the memory model predictable.

A function returning a value can hint that another element is available, creating an iterator, allowing the consumer to request further iteration. Iterators can be stacked, creating a very powerful strategy for working with ranges of values.

```js
function generator(i32 i, i32 count)
{
    if (i < count)
    {
        // Yield the value of i, and provide an instruction for fetching the next value.
        return i -> generator(i + 1, count);
    }
    
    return i; // No more values available
}

function add(i32 addend, i32 value)
{
    return value + addend;
}

function add(i32 addend, (i32 value, ...values))
{
    return value + addend -> add(addend, ...values);
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
    let fromIterator = sum(0, add(10, generator(1, 3)));
    let fromArray = sum(0, add(10, [1, 2, 3]));
    
    return fromIterator + fromArray; // Will return 72
}
```

Both strings and arrays are exposed as iterators.

## Double dispatch

In FluenC, function overloads are resolved based on the real type of the value. There are no interfaces or other abstractions. Instead, functions can return different values from different branches. This in turn will match different consumer functions.

Take the following example:

```js
struct Foo;

function consumer(i32 value)
{
    return value * 10;
}

function consumer(Foo value)
{
    return 32;
}

function producer(i32 value)
{
    if (value > 2)
    {
        return value;
    }
    
    return Foo {};
}
```

Here different overloads of "consumer" will be selected depending on which if-statement we return from.

The secret behind this is the fact that FluenC inverts the standard invocation model. The innermost function will produce N number of results, which will then result in N number of branches being generated.

Arrays work the same way, with each array element producing one branch, allowing us to do things like these:

```js
struct Foo;

function consumer((i32 value, ...values))
{
    return 1 -> consumer(...values);
}

function consumer((i64 value, ...values))
{
    return 2 -> consumer(...values);
}

function consumer(Foo value)
{
    return 3;
}

function sum(i32 product, i32 value)
{
    return product + value;
}

function sum(i32 product, (i32 value, ...values))
{
    return tail sum(product + value, ...values);
}

export int main()
{
    let array = [1, 2i64, Foo {}];
    
    return sum(0, consumer(array)); // Will return 5
}
```

## No "null" value

There are no null values in FluenC. A function may choose to return "nothing", but in contrast to many other languages, the constant value of "nothing" actually has a type - "without". This means that a branch returning "nothing" will need to have a consumer accepting a parameter of type "without". That way we make sure that the scenario where an empty value is returned needs to be handled explicitly.

```js
import i32 open(string apathname, i32 flags);
import i32 getc(i64 stream);

import i64 fdopen(i32 fildes, string mode);

function getValue(i64 stream)
{
    if (getc(stream) > 0)
    {
        return 12;
    }
    
    return nothing;
}

function handleInput(i32 value)
{
    return value;
}

function handleInput(without value)
{
    return 0;
}

export i32 main()
{
    let fd = fdopen(open(@"input.txt", O_RDONLY), @"r");

    return handleInput(getValue(fd));
}
```

## Immutable

Since FluenC is a functional-first language, it makes sense that it's also 100% immutable. Variables can only be declared and initialized, not reassigned. Struct fields can also not be modified. Instead, copies can be made with a given mutation using the `with`-keyword.

```js
struct Foo
{
    x
};

export i32 main()
{
    let var1 = Foo
    {
        x: 1
    };
    
    let var2 = var1 with
    {
        x: 2
    };
    
    return var2.x;
}
```

Since there is no mutable state, loops have to be implemented using recursion. Of course, the language has full support for tail call recursion.

# Status

The language specification and compiler are both under heavy development, and are not ready for general use. That said, a number of different applications have already been created using the language.

# Getting started

The experience when it comes to getting the compiler up and running is probably what is holding back the project the most right now. There are CI builds, exposed as build artifacts, but they are not complete and might or might not run. The compiler is also not self hosted, but relies on clang and a working compiler toolchain for linking and building C code.

The most mature experience is definitely on Linux, and if you are okay with building it yourself, things should work well.

## Building

### Requirements

* CMake 3.10 or later
* A C++ compiler with C++20 support
* LLVM 13 or later

Clone the repo, navigate to the directory, and execute the following commands:

```
cmake .
cmake --build .
```

## Running

After building, the compiler binary can be found in the fcc subfolder of the build directory. As the compiler is not self hosted, and relies on clang, it's  important that a working clang executable can be found.

By default, `fcc` will attempt to find a clang executable in the current directory. This will most likely fail, unless you have installed the compiler.

To instruct `fcc` which clang binary to use, one can set the `CLANG_PATH` environment variable.

```
export CLANG_PATH=/usr/bin/path
```

The `fcc` binary can be invoked with two commands: `init` and `build`.

You can use `init` to initialize a directory with a template project.toml file. The `build` command is used to build a project, and is expected to be invoked inside a directory with a project.toml file.

To build the hello_world application, for example, we can enter the `examples/hello_world` directory and issue the following command (adjust according to your build folder):

```
../../fcc/fcc build
```

If everything is properly set up, you should see output similar to this:

```
Building 'hello_world'...
Linking 'hello_world'...
```

A compiled fluenc binary can be run as any program:

```
./hello_world
```
