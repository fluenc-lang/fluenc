#ifndef BASEVALUE_H
#define BASEVALUE_H

#include "ValueMetadata.h"

class Type;
class EntryPoint;

enum class CloneStrategy
{
	Storage,
	Value
};

enum class ValueId : int64_t
{
	None = -1,
	BaseValue = 0,
	Reference = (1 << 0),
	Scalar = (1 << 2),
	User = (1 << 3),
	Named = (1 << 4),
	String = (1 << 5),
	Lazy = (1 << 6),
	Tuple = (1 << 7),
	Expanded = (1 << 8),
	Function = (1 << 9),
	Expandable = (1 << 10),
	Iterator = (1 << 11),
	Indexed = (1 << 12),
	Forwarded = (1 << 13),
	Placeholder = (1 << 14),
	Without = (1 << 15),
	ExpandedLazy = (1 << 16),
	Buffer = (1 << 17),
};

template <typename T>
inline constexpr ValueId value_id_for = ValueId::None;

class BaseValue;
class ReferenceValue;
class ScalarValue;
class TupleValue;
class WithoutValue;
class PlaceholderValue;
class UserTypeValue;
class NamedValue;
class StringValue;
class LazyValue;
class ExpandedValue;
class FunctionValue;
class ExpandableValue;
class Iterator;
class IndexedValue;
class ForwardedValue;
class ExpandedLazyValue;
class BufferValue;

template <>
inline constexpr ValueId value_id_for<BaseValue> = ValueId::BaseValue;

template <>
inline constexpr ValueId value_id_for<ReferenceValue> = ValueId::Reference;

template <>
inline constexpr ValueId value_id_for<ScalarValue> = ValueId::Scalar;

template <>
inline constexpr ValueId value_id_for<TupleValue> = ValueId::Tuple;

template <>
inline constexpr ValueId value_id_for<WithoutValue> = ValueId::Without;

template <>
inline constexpr ValueId value_id_for<PlaceholderValue> = ValueId::Placeholder;

template <>
inline constexpr ValueId value_id_for<UserTypeValue> = ValueId::User;

template <>
inline constexpr ValueId value_id_for<StringValue> = ValueId::String;

template <>
inline constexpr ValueId value_id_for<LazyValue> = ValueId::Lazy;

template <>
inline constexpr ValueId value_id_for<ExpandedValue> = ValueId::Expanded;

template <>
inline constexpr ValueId value_id_for<FunctionValue> = ValueId::Function;

template <>
inline constexpr ValueId value_id_for<ExpandableValue> = ValueId::Expandable;

template <>
inline constexpr ValueId value_id_for<Iterator> = ValueId::Iterator;

template <>
inline constexpr ValueId value_id_for<IndexedValue> = ValueId::Indexed;

template <>
inline constexpr ValueId value_id_for<ForwardedValue> = ValueId::Forwarded;

template <>
inline constexpr ValueId value_id_for<ExpandedLazyValue> = ValueId::ExpandedLazy;

template <>
inline constexpr ValueId value_id_for<BufferValue> = ValueId::Buffer;

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

		virtual ValueId id() const = 0;

		virtual const Type *type() const = 0;

		virtual const BaseValue *clone(const EntryPoint &entryPoint, CloneStrategy strategy) const = 0;

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

#endif // BASEVALUE_H
