#ifndef FUNCTIONVALUE_H
#define FUNCTIONVALUE_H

#include <vector>

#include "BaseValue.h"

#include "metadata/FunctionValueMetadata.h"

namespace fluenc
{
	struct function_node;
}

class EntryPoint;

class function_value : public BaseValueWithMetadata<FunctionValueMetadata>
{
	public:
		function_value(const std::vector<fluenc::function_node *> functions, const EntryPoint &entryPoint);

                value_id id() const override;

		const Type *type() const override;
                const BaseValue *clone(const EntryPoint &entryPoint, clone_strategy strategy) const override;

		std::vector<fluenc::function_node *> functions() const;

	private:
		std::vector<fluenc::function_node*> m_functions;

		const EntryPoint *m_entryPoint;
};
#endif // FUNCTIONVALUE_H
