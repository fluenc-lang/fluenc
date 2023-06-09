#include <sstream>
#include <numeric>
#include <unordered_map>

#include "types/FunctionType.h"

FunctionType::FunctionType(const std::vector<const Type *> &types, const CallableNode *function)
	: m_types(types)
	, m_function(function)
{
}

TypeId FunctionType::id() const
{
	return TypeId::Function;
}

std::string FunctionType::name() const
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

std::string FunctionType::fullName() const
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

std::vector<const Type *> FunctionType::types() const
{
	return m_types;
}

const CallableNode *FunctionType::function() const
{
	return m_function;
}

llvm::Type *FunctionType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt1Ty(context);
}

FunctionType *FunctionType::get(const std::vector<const Type *> &types, const CallableNode *function)
{
	static std::unordered_map<size_t, FunctionType> cache;

	auto hash = std::accumulate(begin(types), end(types), reinterpret_cast<size_t>(function), [](auto accumulator, auto type)
	{
		return (accumulator ^ (size_t)type) * 1099511628211;
	});

	auto [iterator, _] = cache.try_emplace(hash, types, function);

	return &iterator->second;
}
