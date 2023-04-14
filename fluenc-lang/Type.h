#ifndef TYPE_H
#define TYPE_H

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/DerivedTypes.h>

class EntryPoint;
class IOperatorSet;

enum class TypeId
{
	None = 0,
	Function,
	OpaquePointer,
	Proxy,
	User,
	Prototype,
	Builtin,
	Iterator,
	Aggregate,
	Any,
	Array,
	Expanded,
	Tuple,
	Placeholder,
	Without,
};

template <typename T>
constexpr TypeId type_id_for = TypeId::None;

template <typename T>
class BuiltinType;

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

template <typename T>
constexpr TypeId type_id_for<BuiltinType<T>> = TypeId::Builtin;

template <>
constexpr TypeId type_id_for<FunctionType> = TypeId::Function;

template <>
constexpr TypeId type_id_for<OpaquePointerType> = TypeId::OpaquePointer;

template <>
constexpr TypeId type_id_for<ProxyType> = TypeId::Proxy;

template <>
constexpr TypeId type_id_for<UserType> = TypeId::User;

template <>
constexpr TypeId type_id_for<IPrototype> = TypeId::Prototype;

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

template <typename T, typename U>
T type_cast(U* source)
{
	if (source->id() == type_id_for<std::remove_cv_t<std::remove_pointer_t<T>>>)
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
