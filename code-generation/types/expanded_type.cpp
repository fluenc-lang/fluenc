#include <numeric>
#include <sstream>
#include <unordered_map>

#include "types/expanded_type.hpp"

namespace fluenc::code_generation::types
{
	expanded_type::expanded_type(const std::vector<const base_type*> types)
		: types_(types)
	{
	}

	type_id expanded_type::id() const
	{
		return type_id::expanded;
	}

	std::string expanded_type::name() const
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

	std::string expanded_type::full_name() const
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

	std::vector<const base_type*> expanded_type::types() const
	{
		return types_;
	}

	llvm::Type* expanded_type::storage_type(llvm::LLVMContext& context) const
	{
		std::vector<llvm::Type*> types;

		std::transform(begin(types_), end(types_), std::back_insert_iterator(types), [&](auto type) {
			return type->storage_type(context);
		});

		return llvm::StructType::get(context, types);
	}

	expanded_type* expanded_type::get(const std::vector<const base_type*>& types)
	{
		static std::unordered_map<size_t, expanded_type> cache;

		auto hash = std::accumulate(begin(types), end(types), 14695981039346656037u, [](auto accumulator, auto type) {
			return (accumulator ^ reinterpret_cast<size_t>(type)) * 1099511628211;
		});

		auto [iterator, _] = cache.try_emplace(hash, types);

		return &iterator->second;
	}

	const base_type* expanded_type::iterator_type() const
	{
		return types_[0];
	}
}
