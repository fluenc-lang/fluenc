#ifndef TYPE_H
#define TYPE_H

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/DerivedTypes.h>

class EntryPoint;
class IOperatorSet;

enum class TypeId : int64_t
{
	None = -1,
	Type = 0,
	IPrototype = (1 << 0),
	WithPrototype = (1 << 1) + IPrototype,
	Prototype= (1 << 2) + IPrototype,
	Function = (1 << 3),
	Proxy = (1 << 4),
	User = (1 << 5),
	OpaquePointer = (1 << 6),
	Builtin = (1 << 7),
	Iterator = (1 << 8),
	Aggregate = (1 << 9),
	Any = (1 << 10),
	Array = (1 << 11),
	Expanded = (1 << 12),
	Tuple = (1 << 13),
	Placeholder = (1 << 14),
	Without = (1 << 15) + Builtin,
	String = (1 << 16),
};

template <typename T>
static constexpr TypeId type_id_for = TypeId::None;

template <typename T>
class BuiltinType;

class Type;
class FunctionType;
class OpaquePointerType;
class ProxyType;
class UserType;
class IPrototype;
class IteratorType;
class AggregateType;
class AnyType;
class ArrayType;
class ExpandedType;
class TupleType;
class PlaceholderType;
class WithoutType;
class IBuiltinType;
class WithPrototype;
class Prototype;
class StringType;

template <>
constexpr TypeId type_id_for<Type> = TypeId::Type;

template <typename T>
constexpr TypeId type_id_for<BuiltinType<T>> = TypeId::Builtin;

template <>
constexpr TypeId type_id_for<IBuiltinType> = TypeId::Builtin;

template <>
constexpr TypeId type_id_for<FunctionType> = TypeId::Function;

template <>
constexpr TypeId type_id_for<OpaquePointerType> = TypeId::OpaquePointer;

template <>
constexpr TypeId type_id_for<ProxyType> = TypeId::Proxy;

template <>
constexpr TypeId type_id_for<UserType> = TypeId::User;

template <>
constexpr TypeId type_id_for<IPrototype> = TypeId::IPrototype;

template <>
constexpr TypeId type_id_for<WithPrototype> = TypeId::WithPrototype;

template <>
constexpr TypeId type_id_for<Prototype> = TypeId::Prototype;

template <>
constexpr TypeId type_id_for<IteratorType> = TypeId::Iterator;

template <>
constexpr TypeId type_id_for<AggregateType> = TypeId::Aggregate;

template <>
constexpr TypeId type_id_for<AnyType> = TypeId::Any;

template <>
constexpr TypeId type_id_for<ArrayType> = TypeId::Array;

template <>
constexpr TypeId type_id_for<ExpandedType> = TypeId::Expanded;

template <>
constexpr TypeId type_id_for<TupleType> = TypeId::Tuple;

template <>
constexpr TypeId type_id_for<PlaceholderType> = TypeId::Placeholder;

template <>
constexpr TypeId type_id_for<WithoutType> = TypeId::Without;

template <>
constexpr TypeId type_id_for<StringType> = TypeId::String;

template <typename T, typename U>
T type_cast(U* source)
{
	if (!source)
	{
		return nullptr;
	}

	auto id = static_cast<uint64_t>(type_id_for<std::remove_cv_t<std::remove_pointer_t<T>>>);

	if ((static_cast<uint64_t>(source->id()) & id) == id)
	{
		return reinterpret_cast<T>(source);
	}

	return nullptr;
}

class Type
{
	public:
		virtual TypeId id() const = 0;

		virtual std::string name() const = 0;
		virtual std::string fullName() const;

		virtual llvm::Type *storageType(llvm::LLVMContext &context) const = 0;

		virtual IOperatorSet *operators() const;
};

#endif // TYPE_H
