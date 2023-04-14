#ifndef FUNCTIONVALUE_H
#define FUNCTIONVALUE_H

#include "BaseValue.h"

#include "metadata/FunctionValueMetadata.h"

class CallableNode;
class EntryPoint;

class FunctionValue : public BaseValueWithMetadata<FunctionValueMetadata>
{
	public:
		FunctionValue(const std::vector<const CallableNode *> functions, const EntryPoint &entryPoint);

		ValueId id() const override;

		const Type *type() const override;
		const BaseValue *clone(const EntryPoint &entryPoint, CloneStrategy strategy) const override;

		std::vector<const CallableNode *> functions() const;

	private:
		const std::vector<const CallableNode *> m_functions;

		const EntryPoint *m_entryPoint;
};
#endif // FUNCTIONVALUE_H
