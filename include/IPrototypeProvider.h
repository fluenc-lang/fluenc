#ifndef IPROTOTYPEPROVIDER_H
#define IPROTOTYPEPROVIDER_H

class Stack;
class EntryPoint;
class IPrototype;

class IPrototypeProvider
{
	public:
		virtual IPrototype *provide(const EntryPoint &entryPoint, Stack &values) const = 0;
};

#endif // IPROTOTYPEPROVIDER_H
