#include <sstream>
#include <numeric>
#include <unordered_map>

#include "types/ExpandedType.h"
#include "types/IteratorType.h"

#include "iterators/ExtremitiesIterator.h"

ExpandedType::ExpandedType(const std::vector<const Type *> types)
	: m_types(types)
{
}

std::string ExpandedType::name() const
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

std::string ExpandedType::fullName() const
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

llvm::Type *ExpandedType::storageType(llvm::LLVMContext &context) const
{
	std::vector<llvm::Type *> types;

	std::transform(begin(m_types), end(m_types), std::back_insert_iterator(types), [&](auto type)
	{
		return type->storageType(context);
	});

	return llvm::StructType::get(context, types);
}

int8_t ExpandedType::compatibility(const Type *type, const EntryPoint &entryPoint) const
{
	if (type == this)
	{
		return 0;
	}

	if (dynamic_cast<const IteratorType *>(type))
	{
		return 1;
	}

	if (auto tuple = dynamic_cast<const ExpandedType *>(type))
	{
		if (m_types.size() != tuple->m_types.size())
		{
			return -1;
		}

		int8_t min = 0;
		int8_t max = 0;

		std::transform(begin(m_types), end(m_types), begin(tuple->m_types), extremities_iterator(min, max), [&](auto left, auto right)
		{
			return left->compatibility(right, entryPoint);
		});

		if (min < 0)
		{
			return min;
		}

		return max;
	}

	return -1;
}

ExpandedType *ExpandedType::get(const std::vector<const Type *> &types)
{
	static std::unordered_map<size_t, ExpandedType> cache;

	auto hash = std::accumulate(begin(types), end(types), 14695981039346656037u, [](auto accumulator, auto type)
	{
		return (accumulator ^ (size_t)type) * 1099511628211;
	});

	auto [iterator, _] = cache.try_emplace(hash, types);

	return &iterator->second;
}

const Type *ExpandedType::iteratorType() const
{
	return m_types[0];
}
