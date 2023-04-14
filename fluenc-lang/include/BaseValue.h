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

enum class ValueId
{
	None = 0,
	Reference,
	Scalar,
	User,
	Named,
	String,
	Lazy,
	Tuple,
	Expanded,
	Function,
	Expandable,
	Iterator,
	Indexed,
	Forwarded,
	Placeholder,
	Without,
};

template <typename T>
constexpr ValueId value_id_for = ValueId::None;

class ReferenceValue;
class ScalarValue;
class TupleValue;

template <>
constexpr ValueId value_id_for<ReferenceValue> = ValueId::Reference;

template <>
constexpr ValueId value_id_for<ScalarValue> = ValueId::Scalar;

template <>
constexpr ValueId value_id_for<TupleValue> = ValueId::Tuple;

template <typename T, typename U>
T value_cast(U* source)
{
	if (source->id() == value_id_for<std::remove_cv_t<std::remove_pointer_t<T>>>)
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
		virtual const BaseValue *forward(size_t id) const;

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
