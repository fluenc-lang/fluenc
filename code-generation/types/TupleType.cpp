#include <sstream>
#include <numeric>
#include <unordered_map>

#include "types/TupleType.h"

tuple_type::tuple_type(const std::vector<const Type *> types)
	: m_types(types)
{
}

type_id tuple_type::id() const
{
    return type_id::Tuple;
}

std::string tuple_type::name() const
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

std::string tuple_type::fullName() const
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

std::vector<const Type *> tuple_type::types() const
{
	return m_types;
}

llvm::Type *tuple_type::storageType(llvm::LLVMContext &context) const
{
	std::vector<llvm::Type *> types;

	std::transform(begin(m_types), end(m_types), std::back_insert_iterator(types), [&](auto type)
	{
		return type->storageType(context);
	});

	return llvm::StructType::get(context, types);
}

tuple_type *tuple_type::get(const std::vector<const Type *> &types)
{
    static std::unordered_map<size_t, tuple_type> cache;

	auto hash = std::accumulate(begin(types), end(types), 14695981039346656037u, [](auto accumulator, auto type)
	{
		return (accumulator ^ (size_t)type) * 1099511628211;
	});

	auto [iterator, _] = cache.try_emplace(hash, types);

	return &iterator->second;
}
