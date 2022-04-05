#ifndef FUNCTIONVALUE_H
#define FUNCTIONVALUE_H

#include "BaseValue.h"

class Callable;
class EntryPoint;

class FunctionValue : public BaseValue
{
	public:
		FunctionValue(const Callable *function, const EntryPoint &entryPoint);

		const Type *type() const override;
		const BaseValue *clone(const EntryPoint &entryPoint) const override;

		const Callable *function() const;

	private:
		const Callable *m_function;
		const EntryPoint *m_entryPoint;
};
#endif // FUNCTIONVALUE_H
