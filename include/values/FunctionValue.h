#ifndef FUNCTIONVALUE_H
#define FUNCTIONVALUE_H

#include "BaseValue.h"

#include "metadata/FunctionValueMetadata.h"

class CallableNode;
class EntryPoint;

class FunctionValue : public BaseValueWithMetadata<FunctionValueMetadata>
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
