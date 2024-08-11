#include <numeric>
#include <sstream>
#include <unordered_map>

#include "tuple_type.hpp"

namespace fluenc::code_generation::types
{
	tuple_type::tuple_type(const std::vector<const base_type*> types)
		: types_(types)
	{
	}

	type_id tuple_type::id() const
	{
		return type_id::tuple;
	}

	std::string tuple_type::name() const
	{
		std::stringstream ss;

		ss << "(";

		for (auto i = rbegin(types_); i != rend(types_); i++)
		{
			if (i != rbegin(types_))
			{
				ss << ", ";
			}

			ss << (*i)->name();
		}

		ss << ")";

		return ss.str();
	}

	std::string tuple_type::full_name() const
	{
		std::stringstream ss;

		ss << "(";

		for (auto i = rbegin(types_); i != rend(types_); i++)
		{
			if (i != rbegin(types_))
			{
				ss << ", ";
			}

			ss << (*i)->full_name();
		}

		ss << ")";

		return ss.str();
	}

	std::vector<const base_type*> tuple_type::types() const
	{
		return types_;
	}

	llvm::Type* tuple_type::storage_type(llvm::LLVMContext& context) const
	{
		std::vector<llvm::Type*> types;

		std::transform(begin(types_), end(types_), std::back_insert_iterator(types), [&](auto type) {
			return type->storage_type(context);
		});

		return llvm::StructType::get(context, types);
	}

	tuple_type* tuple_type::get(const std::vector<const base_type*>& types)
	{
		static std::unordered_map<size_t, tuple_type> cache;

		auto hash = std::accumulate(begin(types), end(types), 14695981039346656037u, [](auto accumulator, auto type) {
			return (accumulator ^ (size_t)type) * 1099511628211;
		});

		auto [iterator, _] = cache.try_emplace(hash, types);

		return &iterator->second;
	}
}
