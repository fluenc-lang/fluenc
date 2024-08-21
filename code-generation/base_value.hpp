#pragma once

#include "comfy_stack.hpp"
#include "value_metadata.hpp"

#include "exceptions/invalid_type_exception.hpp"

#include <cstdint>

namespace fluenc::code_generation
{
	struct base_type;
	struct entry_point;

	enum class clone_strategy
	{
		storage,
		value
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
		pre_lazy = (1 << 6),
		tuple = (1 << 7),
		expanded = (1 << 8),
		function = (1 << 9),
		expandable = (1 << 10),
		post_lazy = (1 << 11),
		indexed = (1 << 12),
		forwarded = (1 << 13),
		placeholder = (1 << 14),
		without = (1 << 15),
		expanded_lazy = (1 << 16),
		buffer = (1 << 17),
	};

	template <typename T>
	inline constexpr value_id value_id_for = value_id::none;

	struct base_value;

	namespace values
	{
		struct reference_value;
		struct scalar_value;
		struct tuple_value;
		struct without_value;
		struct placeholder_value;
		struct user_type_value;
		struct named_value;
		struct string_value;
		struct pre_lazy_value;
		struct expanded_value;
		struct function_value;
		struct expandable_value;
		struct post_lazy_value;
		struct indexed_value;
		struct forwarded_value;
		struct expanded_lazy_value;
		struct buffer_value;
	}

	template <>
	inline constexpr value_id value_id_for<base_value> = value_id::base_value;

	template <>
	inline constexpr value_id value_id_for<values::reference_value> = value_id::reference;

	template <>
	inline constexpr value_id value_id_for<values::scalar_value> = value_id::scalar;

	template <>
	inline constexpr value_id value_id_for<values::tuple_value> = value_id::tuple;

	template <>
	inline constexpr value_id value_id_for<values::without_value> = value_id::without;

	template <>
	inline constexpr value_id value_id_for<values::placeholder_value> = value_id::placeholder;

	template <>
	inline constexpr value_id value_id_for<values::user_type_value> = value_id::user;

	template <>
	inline constexpr value_id value_id_for<values::string_value> = value_id::string;

	template <>
	inline constexpr value_id value_id_for<values::pre_lazy_value> = value_id::pre_lazy;

	template <>
	inline constexpr value_id value_id_for<values::expanded_value> = value_id::expanded;

	template <>
	inline constexpr value_id value_id_for<values::function_value> = value_id::function;

	template <>
	inline constexpr value_id value_id_for<values::expandable_value> = value_id::expandable;

	template <>
	inline constexpr value_id value_id_for<values::post_lazy_value> = value_id::post_lazy;

	template <>
	inline constexpr value_id value_id_for<values::indexed_value> = value_id::indexed;

	template <>
	inline constexpr value_id value_id_for<values::forwarded_value> = value_id::forwarded;

	template <>
	inline constexpr value_id value_id_for<values::expanded_lazy_value> = value_id::expanded_lazy;

	template <>
	inline constexpr value_id value_id_for<values::buffer_value> = value_id::buffer;

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

	struct base_value
	{
		virtual ~base_value() = default;

		virtual value_id id() const = 0;

		virtual const base_type* type() const = 0;

		virtual const base_value* clone(const entry_point& entryPoint, clone_strategy strategy) const = 0;

		virtual const value_metadata& metadata() const = 0;
	};

	template <typename TMetadata>
	struct base_value_with_metadata : public base_value
	{
		static const value_metadata& static_metadata()
		{
			static TMetadata instance;

			return instance;
		}

		const value_metadata& metadata() const override
		{
			return static_metadata();
		}
	};

	using value_stack_t = comfy_stack<const base_value*>;

	template <typename TValue>
	const TValue* require(value_stack_t& stack, const std::shared_ptr<peg::Ast>& ast)
	{
		auto value = stack.pop();

		if (auto casted = value_cast<const TValue*>(value))
		{
			return casted;
		}

		auto& expectedMetadata = TValue::static_metadata();
		auto& actualMetadata = value->metadata();

		throw invalid_type_exception(ast, expectedMetadata.name(), actualMetadata.name());
	}

	template <typename TValue>
	const TValue* request(value_stack_t& stack)
	{
		if (stack.size() <= 0)
		{
			return nullptr;
		}

		auto value = stack.pop();

		if (auto casted = value_cast<const TValue*>(value))
		{
			return casted;
		}

		return nullptr;
	}
}
