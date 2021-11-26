#include <sstream>
#include <numeric>

#include "TupleType.h"
#include "Utility.h"
#include "AllIterator.h"

TupleType::TupleType(const std::vector<const Type *> types)
	: m_types(types)
{
}

std::string TupleType::tag() const
{
	std::stringstream ss;

	std::transform(begin(m_types), end(m_types), std::ostream_iterator<std::string>(ss), [](auto type)
	{
		return type->tag();
	});

	return ss.str();
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

bool TupleType::is(const Type *type, const EntryPoint &entryPoint) const
{
	if (auto tuple = dynamic_cast<const TupleType *>(type))
	{
		if (m_types.size() != tuple->m_types.size())
		{
			return false;
		}

		auto result = true;

		std::transform(begin(m_types), end(m_types), begin(tuple->m_types), all_true(result), [&](auto left, auto right)
		{
			return left->is(right, entryPoint);
		});

		return result;
	}

	return false;
}

TupleType *TupleType::get(const std::vector<const Type *> types)
{
	static std::unordered_map<size_t, TupleType> cache;

	auto hash = std::accumulate(begin(types), end(types), 14695981039346656037u, [](auto accumulator, auto type)
	{
		return (accumulator ^ (size_t)type) * 1099511628211;
	});

	auto [iterator, _] = cache.try_emplace(hash, types);

	return &iterator->second;
}
