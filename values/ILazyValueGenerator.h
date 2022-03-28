#ifndef ILAZYVALUEGENERATOR_H
#define ILAZYVALUEGENERATOR_H

class IIteratable;
class EntryPoint;

class ILazyValueGenerator
{
	public:
		virtual IIteratable *generate(const EntryPoint &entryPoint) const = 0;
};

#endif // ILAZYVALUEGENERATOR_H
