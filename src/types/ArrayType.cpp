#include <sstream>
#include <numeric>

#include "types/ArrayType.h"
#include "Utility.h"

ArrayType::ArrayType(std::vector<const Type *> types)
	: m_types(types)
{
}

std::string ArrayType::name() const
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

std::string ArrayType::fullName() const
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

llvm::Type *ArrayType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt8PtrTy(context);
}

int8_t ArrayType::compatibility(const Type *type, const EntryPoint &entryPoint) const
{
	UNUSED(type);
	UNUSED(entryPoint);

	return 0;
}

ArrayType *ArrayType::get(const std::vector<const Type *> &types)
{
	static std::unordered_map<size_t, ArrayType> cache;

	auto hash = std::accumulate(begin(types), end(types), 14695981039346656037u, [](auto accumulator, auto type)
	{
		return (accumulator ^ (size_t)type) * 1099511628211;
	});

	auto [iterator, _] = cache.try_emplace(hash, types);

	return &iterator->second;
}
