#ifndef FUNCTIONVALUE_H
#define FUNCTIONVALUE_H

#include "BaseValue.h"

class CallableNode;
class EntryPoint;

class FunctionValue : public BaseValue
{
	public:
		FunctionValue(const CallableNode *function, const EntryPoint &entryPoint);

		const Type *type() const override;
		const BaseValue *clone(const EntryPoint &entryPoint) const override;

		const CallableNode *function() const;

	private:
		const CallableNode *m_function;
		const EntryPoint *m_entryPoint;
};
#endif // FUNCTIONVALUE_H
