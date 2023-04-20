#include <sstream>
#include <numeric>
#include <unordered_map>

#include "types/TupleType.h"

TupleType::TupleType(const std::vector<const Type *> types)
	: m_types(types)
{
}

TypeId TupleType::id() const
{
	return TypeId::Tuple;
}

std::string TupleType::name() const
{
	std::stringstream ss;

	ss << "(";

	for (auto i = rbegin(m_types); i != rend(m_types); i++)
	{
		if (i != rbegin(m_types))
		{
			ss << ", ";
		}

		ss << (*i)->name();
	}

	ss << ")";

	return ss.str();
}

std::string TupleType::fullName() const
{
	std::stringstream ss;

	ss << "(";

	for (auto i = rbegin(m_types); i != rend(m_types); i++)
	{
		if (i != rbegin(m_types))
		{
			ss << ", ";
		}

		ss << (*i)->fullName();
	}

	ss << ")";

	return ss.str();
}

std::vector<const Type *> TupleType::types() const
{
	return m_types;
}

llvm::Type *TupleType::storageType(llvm::LLVMContext &context) const
{
	std::vector<llvm::Type *> types;

	std::transform(begin(m_types), end(m_types), std::back_insert_iterator(types), [&](auto type)
	{
		return type->storageType(context);
	});

	return llvm::StructType::get(context, types);
}

TupleType *TupleType::get(const std::vector<const Type *> &types)
{
	static std::unordered_map<size_t, TupleType> cache;

	auto hash = std::accumulate(begin(types), end(types), 14695981039346656037u, [](auto accumulator, auto type)
	{
		return (accumulator ^ (size_t)type) * 1099511628211;
	});

	auto [iterator, _] = cache.try_emplace(hash, types);

	return &iterator->second;
}
