#ifndef ILAZYVALUEGENERATOR_H
#define ILAZYVALUEGENERATOR_H

#include <string>

class IIteratable;
class EntryPoint;
class Type;

enum class GenerationMode
{
	Regular = 0,
	DryRun
};

class ILazyValueGenerator
{
	public:
		virtual const IIteratable *generate(const EntryPoint &entryPoint, GenerationMode mode) const = 0;

		virtual const ILazyValueGenerator *clone(const EntryPoint &entryPoint) const = 0;
		virtual const ILazyValueGenerator *forward(size_t id) const = 0;

		virtual const Type *type() const = 0;
};

#endif // ILAZYVALUEGENERATOR_H
