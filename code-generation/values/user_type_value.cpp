#include "named_value.hpp"
#include "user_type_value.hpp"

#include "types/default_prototype.hpp"
#include "types/user_type.hpp"

namespace fluenc::code_generation::values
{
	user_type_value::user_type_value(const types::structure_type* type, const std::vector<const named_value*>& values)
		: type_(type)
		, values_(values)
	{
	}

	value_id user_type_value::id() const
	{
		return value_id::user;
	}

	const base_type* user_type_value::type() const
	{
		std::vector<const base_type*> elementTypes;

		std::transform(begin(values_), end(values_), std::back_insert_iterator(elementTypes), [](auto value) {
			return value->type();
		});

		return types::user_type::get(type_, elementTypes);
	}

	const types::structure_type* user_type_value::prototype() const
	{
		return type_;
	}

	const base_value* user_type_value::clone(const entry_point& entryPoint, clone_strategy strategy) const
	{
		std::vector<const named_value*> values;

		std::transform(begin(values_), end(values_), std::back_inserter(values), [&](auto value) {
			auto cloned = value->clone(entryPoint, strategy);

			return static_cast<const named_value*>(cloned);
		});

		return new user_type_value(type_, values);
	}

	std::vector<const named_value*> user_type_value::fields() const
	{
		return values_;
	}
}
