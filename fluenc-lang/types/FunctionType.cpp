#include <sstream>
#include <numeric>
#include <unordered_map>

#include "types/FunctionType.h"
#include "AllIterator.h"

#include "iterators/ExtremitiesIterator.h"

FunctionType::FunctionType(const std::vector<const Type *> &types, const CallableNode *function)
	: m_types(types)
	, m_function(function)
{
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

llvm::Type *FunctionType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt1Ty(context);
}

int8_t FunctionType::compatibility(const Type *type, const EntryPoint &entryPoint) const
{
	if (type == this)
	{
		return 0;
	}

	auto other = dynamic_cast<const FunctionType *>(type);

	if (!other)
	{
		return -1;
	}

	if (m_types.size() != other->m_types.size())
	{
		return -1;
	}

	int8_t min = 0;
	int8_t max = 0;

	std::transform(begin(m_types), end(m_types), begin(other->m_types), extremities_iterator(min, max), [&](auto left, auto right)
	{
		return left->compatibility(right, entryPoint);
	});

	if (min < 0)
	{
		return min;
	}

	if (m_function != other->m_function)
	{
		return std::max(static_cast<int8_t>(1), max);
	}

	return max;
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
