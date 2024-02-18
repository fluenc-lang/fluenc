#pragma once

#include <vector>

#include "BaseValue.h"

#include "metadata/UserTypeValueMetadata.h"

class named_value;
class IPrototype;

class structure_type;

class user_type_value : public BaseValueWithMetadata<UserTypeValueMetadata>
{
	public:
		user_type_value(const structure_type *type, const std::vector<const named_value *> &values);

                value_id id() const override;

		const Type *type() const override;
                const BaseValue *clone(const EntryPoint &entryPoint, clone_strategy strategy) const override;

		const structure_type *prototype() const;

                std::vector<const named_value *> fields() const;

	private:
		const structure_type *m_type;

                std::vector<const named_value *> m_values;
};
