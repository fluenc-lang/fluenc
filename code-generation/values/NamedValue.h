#pragma once

#include <llvm/IR/BasicBlock.h>

#include "BaseValue.h"

#include "metadata/NamedValueMetadata.h"

class named_value : public BaseValueWithMetadata<NamedValueMetadata>
{
	public:
        named_value(const std::string &name, const BaseValue *value);

                value_id id() const override;

		std::string name() const;

		const BaseValue *value() const;

		const Type *type() const override;
                const BaseValue *clone(const EntryPoint &entryPoint, clone_strategy strategy) const override;

	private:
		std::string m_name;

		const BaseValue *m_value;
};
