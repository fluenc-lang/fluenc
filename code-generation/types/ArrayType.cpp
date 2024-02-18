#include <sstream>
#include <numeric>
#include <unordered_map>

#include "ArrayType.h"
#include "ArrayOperatorSet.h"

array_type::array_type(std::vector<const Type *> types)
	: m_types(types)
{
}

type_id array_type::id() const
{
    return type_id::Array;
}

std::string array_type::name() const
{
	std::stringstream ss;

	ss << "[";

	for (auto i = begin(m_types); i != end(m_types); i++)
	{
		if (i != begin(m_types))
		{
			ss << ", ";
		}

		ss << (*i)->name();
	}

	ss << "]";

	return ss.str();
}

std::string array_type::fullName() const
{
	std::stringstream ss;

	ss << "[";

	for (auto i = begin(m_types); i != end(m_types); i++)
	{
		if (i != begin(m_types))
		{
			ss << ", ";
		}

		ss << (*i)->fullName();
	}

	ss << "]";

	return ss.str();
}

llvm::Type *array_type::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt8PtrTy(context);
}

IOperatorSet *array_type::operators() const
{
	static ArrayOperatorSet operators;

	return &operators;
}

std::vector<const Type *> array_type::types() const
{
	return m_types;
}

array_type *array_type::get(const std::vector<const Type *> &types)
{
	static std::unordered_map<size_t, array_type> cache;

	auto hash = std::accumulate(begin(types), end(types), 14695981039346656037u, [](auto accumulator, auto type)
	{
		return (accumulator ^ (size_t)type) * 1099511628211;
	});

	auto [iterator, _] = cache.try_emplace(hash, types);

	return &iterator->second;
}
