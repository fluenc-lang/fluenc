#pragma once

#include "ValueMetadata.h"

#include <cstdint>

class Type;
class EntryPoint;

enum class clone_strategy
{
	Storage,
	Value
};

enum class value_id : int64_t
{
	none = -1,
	base_value = 0,
	reference = (1 << 0),
	scalar = (1 << 2),
	user = (1 << 3),
	named = (1 << 4),
	string = (1 << 5),
	lazy = (1 << 6),
	tuple = (1 << 7),
	expanded = (1 << 8),
	function = (1 << 9),
	expandable = (1 << 10),
	iterator = (1 << 11),
	indexed = (1 << 12),
	forwarded = (1 << 13),
	placeholder = (1 << 14),
	without = (1 << 15),
	expanded_lazy = (1 << 16),
	buffer = (1 << 17),
};

template <typename T>
inline constexpr value_id value_id_for = value_id::none;

class BaseValue;
class reference_value;
class scalar_value;
class tuple_value;
class without_value;
class placeholder_value;
class user_type_value;
class named_value;
class StringValue;
class lazy_value;
class ExpandedValue;
class function_value;
class ExpandableValue;
class Iterator;
class indexed_value;
class ForwardedValue;
class ExpandedLazyValue;
class BufferValue;

template <>
inline constexpr value_id value_id_for<BaseValue> = value_id::base_value;

template <>
inline constexpr value_id value_id_for<reference_value> = value_id::reference;

template <>
inline constexpr value_id value_id_for<scalar_value> = value_id::scalar;

template <>
inline constexpr value_id value_id_for<tuple_value> = value_id::tuple;

template <>
inline constexpr value_id value_id_for<without_value> = value_id::without;

template <>
inline constexpr value_id value_id_for<placeholder_value> = value_id::placeholder;

template <>
inline constexpr value_id value_id_for<user_type_value> = value_id::user;

template <>
inline constexpr value_id value_id_for<StringValue> = value_id::string;

template <>
inline constexpr value_id value_id_for<lazy_value> = value_id::lazy;

template <>
inline constexpr value_id value_id_for<ExpandedValue> = value_id::expanded;

template <>
inline constexpr value_id value_id_for<function_value> = value_id::function;

template <>
inline constexpr value_id value_id_for<ExpandableValue> = value_id::expandable;

template <>
inline constexpr value_id value_id_for<Iterator> = value_id::iterator;

template <>
inline constexpr value_id value_id_for<indexed_value> = value_id::indexed;

template <>
inline constexpr value_id value_id_for<ForwardedValue> = value_id::forwarded;

template <>
inline constexpr value_id value_id_for<ExpandedLazyValue> = value_id::expanded_lazy;

template <>
inline constexpr value_id value_id_for<BufferValue> = value_id::buffer;

template <typename T, typename U>
T value_cast(U* source)
{
	if (!source)
	{
		return nullptr;
	}

	auto id = static_cast<uint64_t>(value_id_for<std::remove_cv_t<std::remove_pointer_t<T>>>);

	if ((static_cast<uint64_t>(source->id()) & id) == id)
	{
		return reinterpret_cast<T>(source);
	}

	return nullptr;
}

class BaseValue
{
	public:
		virtual ~BaseValue() = default;

		virtual value_id id() const = 0;

		virtual const Type *type() const = 0;

		virtual const BaseValue *clone(const EntryPoint &entryPoint, clone_strategy strategy) const = 0;

		virtual const ValueMetadata &metadata() const = 0;
};

template<typename TMetadata>
struct BaseValueWithMetadata : public BaseValue
{
	static const ValueMetadata &staticMetadata()
	{
		static TMetadata instance;

		return instance;
	}

	const ValueMetadata &metadata() const override
	{
		return staticMetadata();
	}
};
