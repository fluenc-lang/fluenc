#include <numeric>
#include <sstream>
#include <unordered_map>

#include "function_type.hpp"

namespace fluenc::code_generation::types
{
	function_type::function_type(const std::vector<const base_type*>& types, function_node* function)
		: types_(types)
		, function_(function)
	{
	}

	type_id function_type::id() const
	{
		return type_id::function;
	}

	std::string function_type::name() const
	{
		std::stringstream stream;

		stream << "function (";

		for (auto i = begin(types_); i != end(types_); i++)
		{
			if (i != begin(types_))
			{
				stream << ", ";
			}

			stream << (*i)->name();
		}

		stream << ")";

		return stream.str();
	}

	std::string function_type::full_name() const
	{
		std::stringstream stream;

		stream << "function (";

		for (auto i = begin(types_); i != end(types_); i++)
		{
			if (i != begin(types_))
			{
				stream << ", ";
			}

			stream << (*i)->full_name();
		}

		stream << ")";

		return stream.str();
	}

	std::vector<const base_type*> function_type::types() const
	{
		return types_;
	}

	function_node* function_type::function() const
	{
		return function_;
	}

	llvm::Type* function_type::storage_type(llvm::LLVMContext& context) const
	{
		return llvm::Type::getInt1Ty(context);
	}

	function_type* function_type::get(const std::vector<const base_type*>& types, function_node* function)
	{
		static std::unordered_map<size_t, function_type> cache;

		auto hash = std::
			accumulate(begin(types), end(types), reinterpret_cast<size_t>(function), [](auto accumulator, auto type) {
				return (accumulator ^ reinterpret_cast<size_t>(type)) * 1099511628211;
			});

		auto [iterator, _] = cache.try_emplace(hash, types, function);

		return &iterator->second;
	}
}
