#include <sstream>
#include <numeric>
#include <unordered_map>

#include "types/FunctionType.h"

function_type::function_type(const std::vector<const Type *> &types, fluenc::function_node *function)
	: m_types(types)
	, m_function(function)
{
}

type_id function_type::id() const
{
    return type_id::Function;
}

std::string function_type::name() const
{
	std::stringstream ss;

	ss << "function (";

	for (auto i = begin(m_types); i != end(m_types); i++)
	{
		if (i != begin(m_types))
		{
			ss << ", ";
		}

		ss << (*i)->name();
	}

	ss << ")";

	return ss.str();
}

std::string function_type::fullName() const
{
	std::stringstream ss;

	ss << "function (";

	for (auto i = begin(m_types); i != end(m_types); i++)
	{
		if (i != begin(m_types))
		{
			ss << ", ";
		}

		ss << (*i)->fullName();
	}

	ss << ")";

	return ss.str();
}

std::vector<const Type *> function_type::types() const
{
	return m_types;
}

fluenc::function_node *function_type::function() const
{
	return m_function;
}

llvm::Type *function_type::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt1Ty(context);
}

function_type *function_type::get(const std::vector<const Type *> &types, fluenc::function_node* function)
{
	static std::unordered_map<size_t, function_type> cache;

	auto hash = std::accumulate(begin(types), end(types), reinterpret_cast<size_t>(function), [](auto accumulator, auto type)
	{
		return (accumulator ^ (size_t)type) * 1099511628211;
	});

	auto [iterator, _] = cache.try_emplace(hash, types, function);

	return &iterator->second;
}
