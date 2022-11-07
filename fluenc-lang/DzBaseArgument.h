#ifndef DZBASEARGUMENT_H
#define DZBASEARGUMENT_H

class Type;
class EntryPoint;

class DzBaseArgument
{
	public:
		virtual Type *type(const EntryPoint &entryPoint) const = 0;
};

#endif // DZBASEARGUMENT_H
