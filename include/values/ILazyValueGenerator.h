#ifndef ILAZYVALUEGENERATOR_H
#define ILAZYVALUEGENERATOR_H

class IIteratable;
class EntryPoint;
class Type;

class ILazyValueGenerator
{
	public:
		virtual const IIteratable *generate(const EntryPoint &entryPoint) const = 0;

		virtual const Type *type() const = 0;
};

#endif // ILAZYVALUEGENERATOR_H
