# FluenC

FluenC is a C-inspired, functional-first language, with a focus on simplicity.

# Hello world in FluenC

```js
import int puts(string str);

export int main()
{
    return puts("Hello world!");
}
```

# Memory management

Perhaps the biggest difference between FluenC and other languages is the fact that the memory model of FluenC is so simple. The language is designed in such a way that memory is never allocated in runtime, be it stack allocation or heap allocation. Instead, each function parameter and return value has a fixed memory address, with storage being allocated upfront at application startup. This makes the memory usage of a basic FluenC program very predictable.

It's still possible to perform dynamic memory allocation by utilizing the C interop facilities of the language, which can be useful when extending the core libraries for example.

# Iterators

Since there are no allocations, consumption of arrays and similar structures works slightly different from other languages. You only ever work with one element at a time, making the memory model predictable.

A function returning a value can hint that another element is available, creating an iterator, allowing the consumer to request further iteration. Iterators can be stacked, creating a very powerful strategy for working with ranges of values.

# Double dispatch

In FluenC, function overloads are resolved based on the real type of the value. There are no interfaces or other abstractions. Instead, functions can return different values from different branches. This in turn will match different consumer functions.

Take the following example:

(to be written)

Here different overloads of "consumer" will be selected depending on which if-statement we return from.

The secret behind this is the fact that FluenC inverts the standard invocation model. The innermost function will produce N number of results, which will then result in N number of branches being generated.

# No "null" value

There are no null values in FluenC. A function may choose to return "nothing", but in contrast to many other languages, the constant value of "nothing" actually has a type - "without". This means that a branch returning "nothing" will need to have a consumer accepting a parameter of type "without". That way we make sure that the scenario where an empty value is returned needs to be handled explicitly.
