#include "string_value.hpp"
#include "pre_lazy_value.hpp"
#include "reference_value.hpp"

#include "generators/string_iterator_generator.hpp"

#include "types/string_type.hpp"

namespace fluenc::code_generation::values
{
	string_value::string_value(const reference_value* address, const reference_value* length)
		: address_(address)
		, length_(length)
	{
	}

	value_id string_value::id() const
	{
		return value_id::string;
	}

	const reference_value* string_value::reference() const
	{
		return address_;
	}

	const reference_value* string_value::length() const
	{
		return length_;
	}

	const pre_lazy_value* string_value::iterator(const entry_point& entry_point) const
	{
		auto generator = new generators::string_iterator_generator(address_, length_);

		return new pre_lazy_value(generator, entry_point);
	}

	const base_type* string_value::type() const
	{
		return types::string_type::instance();
	}

	const base_value* string_value::clone(const entry_point& entry_point, clone_strategy strategy) const
	{
		auto subject = static_cast<const reference_value*>(address_->clone(entry_point, strategy));

		return new string_value(subject, length_);
	}
}
