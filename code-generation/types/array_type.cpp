#include <numeric>
#include <sstream>
#include <unordered_map>

#include "array_operator_set.hpp"
#include "array_type.hpp"

namespace fluenc::code_generation::types
{
	array_type::array_type(std::vector<const base_type*> types)
		: types_(types)
	{
	}

	type_id array_type::id() const
	{
		return type_id::array;
	}

	std::string array_type::name() const
	{
		std::stringstream ss;

		ss << "[";

		for (auto i = begin(types_); i != end(types_); i++)
		{
			if (i != begin(types_))
			{
				ss << ", ";
			}

			ss << (*i)->name();
		}

		ss << "]";

		return ss.str();
	}

	std::string array_type::full_name() const
	{
		std::stringstream ss;

		ss << "[";

		for (auto i = begin(types_); i != end(types_); i++)
		{
			if (i != begin(types_))
			{
				ss << ", ";
			}

			ss << (*i)->full_name();
		}

		ss << "]";

		return ss.str();
	}

	llvm::Type* array_type::storage_type(llvm::LLVMContext& context) const
	{
		return llvm::Type::getInt8PtrTy(context);
	}

	operator_set* array_type::operators() const
	{
		static array_operator_set operators;

		return &operators;
	}

	std::vector<const base_type*> array_type::types() const
	{
		return types_;
	}

	array_type* array_type::get(const std::vector<const base_type*>& types)
	{
		static std::unordered_map<size_t, array_type> cache;

		auto hash = std::accumulate(begin(types), end(types), 14695981039346656037u, [](auto accumulator, auto type) {
			return (accumulator ^ (size_t)type) * 1099511628211;
		});

		auto [iterator, _] = cache.try_emplace(hash, types);

		return &iterator->second;
	}
}
