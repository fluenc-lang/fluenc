#pragma once

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

namespace fluenc::code_generation
{
	struct entry_point;
	struct operator_set;

	enum class type_id : int64_t
	{
		none = -1,
		type = 0,
		prototype = (1 << 0),
		with_prototype = (1 << 1) + prototype,
		default_prototype = (1 << 2) + prototype,
		function = (1 << 3),
		proxy = (1 << 4),
		user = (1 << 5),
		opaque_pointer = (1 << 6),
		builtin = (1 << 7),
		iterator = (1 << 8),
		aggregate = (1 << 9),
		any = (1 << 10),
		array = (1 << 11),
		expanded = (1 << 12),
		tuple = (1 << 13),
		placeholder = (1 << 14),
		without = (1 << 15) + builtin,
		string = (1 << 16),
		buffer = (1 << 17),
		void_interop = (1 << 18),
	};

	template <typename T>
	inline constexpr type_id type_id_for = type_id::none;

	struct base_type;

	namespace types
	{
		template <typename T>
		struct builtin_type;

		struct function_type;
		struct opaque_pointer_type;
		struct proxy_type;
		struct user_type;
		struct iterator_type;
		struct aggregate_type;
		struct any_type;
		struct array_type;
		struct expanded_type;
		struct tuple_type;
		struct placeholder_type;
		struct without_type;
		struct builtin_base_type;
		struct string_type;
		struct buffer_type;
		struct void_interop_type;
		struct structure_type;
	}

	template <>
	inline constexpr type_id type_id_for<base_type> = type_id::type;

	template <typename T>
	inline constexpr type_id type_id_for<types::builtin_type<T>> = type_id::builtin;

	template <>
	inline constexpr type_id type_id_for<types::builtin_base_type> = type_id::builtin;

	template <>
	inline constexpr type_id type_id_for<types::function_type> = type_id::function;

	template <>
	inline constexpr type_id type_id_for<types::opaque_pointer_type> = type_id::opaque_pointer;

	template <>
	inline constexpr type_id type_id_for<types::proxy_type> = type_id::proxy;

	template <>
	inline constexpr type_id type_id_for<types::user_type> = type_id::user;

	template <>
	inline constexpr type_id type_id_for<types::structure_type> = type_id::default_prototype;

	template <>
	inline constexpr type_id type_id_for<types::iterator_type> = type_id::iterator;

	template <>
	inline constexpr type_id type_id_for<types::aggregate_type> = type_id::aggregate;

	template <>
	inline constexpr type_id type_id_for<types::any_type> = type_id::any;

	template <>
	inline constexpr type_id type_id_for<types::array_type> = type_id::array;

	template <>
	inline constexpr type_id type_id_for<types::expanded_type> = type_id::expanded;

	template <>
	inline constexpr type_id type_id_for<types::tuple_type> = type_id::tuple;

	template <>
	inline constexpr type_id type_id_for<types::placeholder_type> = type_id::placeholder;

	template <>
	inline constexpr type_id type_id_for<types::without_type> = type_id::without;

	template <>
	inline constexpr type_id type_id_for<types::string_type> = type_id::string;

	template <>
	inline constexpr type_id type_id_for<types::buffer_type> = type_id::buffer;

	template <>
	inline constexpr type_id type_id_for<types::void_interop_type> = type_id::void_interop;

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

	struct base_type
	{
		virtual type_id id() const = 0;

		virtual std::string name() const = 0;
		virtual std::string full_name() const;

		virtual llvm::Type* storage_type(llvm::LLVMContext& context) const = 0;

		virtual operator_set* operators() const;
	};
}
