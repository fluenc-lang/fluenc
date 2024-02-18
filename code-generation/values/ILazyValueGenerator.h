#ifndef ILAZYVALUEGENERATOR_H
#define ILAZYVALUEGENERATOR_H

#include <string>

#include "BaseValue.h"

class IIteratable;
class EntryPoint;
class Type;

class fragment;

enum class GenerationMode
{
	Regular = 0,
	DryRun
};

class ILazyValueGenerator
{
	public:
		virtual const fragment *generate(const EntryPoint &entryPoint, GenerationMode mode) const = 0;

        virtual const ILazyValueGenerator *clone(const EntryPoint &entryPoint, clone_strategy strategy) const = 0;

		virtual const Type *type() const = 0;
};

#endif // ILAZYVALUEGENERATOR_H
