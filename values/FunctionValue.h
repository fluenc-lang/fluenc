#ifndef FUNCTIONVALUE_H
#define FUNCTIONVALUE_H

#include "BaseValue.h"

class DzCallable;
class EntryPoint;

class FunctionValue : public BaseValue
{
	public:
		FunctionValue(const DzCallable *function, const EntryPoint &entryPoint);

		const Type *type() const override;
		const BaseValue *clone(const EntryPoint &entryPoint) const override;

		const DzCallable *function() const;

	private:
		const DzCallable *m_function;
		const EntryPoint *m_entryPoint;
};
#endif // FUNCTIONVALUE_H
