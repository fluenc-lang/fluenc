#pragma once

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/DerivedTypes.h>

class EntryPoint;
class IOperatorSet;

enum class type_id : int64_t
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
	Buffer = (1 << 17),
	Void = (1 << 18),
};

template <typename T>
inline constexpr type_id type_id_for = type_id::None;

template <typename T>
class BuiltinType;

class Type;
class function_type;
class opaque_pointer_type;
class proxy_type;
class user_type;
class iterator_type;
class aggregate_type;
class any_type;
class array_type;
class expanded_type;
class tuple_type;
class placeholder_type;
class without_type;
class IBuiltinType;
class string_type;
class buffer_type;
class void_type;

namespace fluenc
{
    struct struct_node;
}

class structure_type;

template <>
inline constexpr type_id type_id_for<Type> = type_id::Type;

template <typename T>
inline constexpr type_id type_id_for<BuiltinType<T>> = type_id::Builtin;

template <>
inline constexpr type_id type_id_for<IBuiltinType> = type_id::Builtin;

template <>
inline constexpr type_id type_id_for<function_type> = type_id::Function;

template <>
inline constexpr type_id type_id_for<opaque_pointer_type> = type_id::OpaquePointer;

template <>
inline constexpr type_id type_id_for<proxy_type> = type_id::Proxy;

template <>
inline constexpr type_id type_id_for<user_type> = type_id::User;

template <>
inline constexpr type_id type_id_for<structure_type> = type_id::Prototype;

template <>
inline constexpr type_id type_id_for<iterator_type> = type_id::Iterator;

template <>
inline constexpr type_id type_id_for<aggregate_type> = type_id::Aggregate;

template <>
inline constexpr type_id type_id_for<any_type> = type_id::Any;

template <>
inline constexpr type_id type_id_for<array_type> = type_id::Array;

template <>
inline constexpr type_id type_id_for<expanded_type> = type_id::Expanded;

template <>
inline constexpr type_id type_id_for<tuple_type> = type_id::Tuple;

template <>
inline constexpr type_id type_id_for<placeholder_type> = type_id::Placeholder;

template <>
inline constexpr type_id type_id_for<without_type> = type_id::Without;

template <>
inline constexpr type_id type_id_for<string_type> = type_id::String;

template <>
inline constexpr type_id type_id_for<buffer_type> = type_id::Buffer;

template <>
inline constexpr type_id type_id_for<void_type> = type_id::Void;

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
		virtual type_id id() const = 0;

		virtual std::string name() const = 0;
		virtual std::string fullName() const;

		virtual llvm::Type *storageType(llvm::LLVMContext &context) const = 0;

		virtual IOperatorSet *operators() const;
};
