#include <sstream>
#include <numeric>
#include <unordered_map>

#include "Utility.h"

#include "types/AnyType.h"
#include "types/UserType.h"

#include "iterators/ExtremitiesIterator.h"

UserType::UserType(const Type *prototype, const std::vector<const Type *> &elementTypes)
	: m_prototype(prototype)
	, m_elementTypes(elementTypes)
{
}

std::string UserType::name() const
{
	return m_prototype->name();
}

std::string UserType::fullName() const
{
	std::ostringstream stream;

	stream << m_prototype->name();

	if (m_elementTypes.size() > 0)
	{
		stream << "<";

		for (auto i = begin(m_elementTypes)
			; i != end(m_elementTypes)
			; i++
			)
		{
			if (i != begin(m_elementTypes))
			{
				stream << ", ";
			}

			stream << (*i)->fullName();
		}

		stream << ">";
	}

	return stream.str();
}

llvm::Type *UserType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt8PtrTy(context);
}

int8_t UserType::compatibility(const Type *type, const EntryPoint &entryPoint) const
{
	auto other = dynamic_cast<const UserType *>(type);

	if (!other)
	{
		return m_prototype->compatibility(type, entryPoint);
	}

	if (m_elementTypes.size() != other->m_elementTypes.size())
	{
		return -1;
	}

	int8_t min = 0;
	int8_t max = 0;

	std::transform(begin(m_elementTypes), end(m_elementTypes), begin(other->m_elementTypes), extremities_iterator(min, max), [&](auto left, auto right)
	{
		return left->compatibility(right, entryPoint);
	});

	if (min < 0 || max > 0)
	{
		return m_prototype->compatibility(type, entryPoint);
	}

	return 0;
}

UserType *UserType::get(const Type *prototype, const std::vector<const Type *> &elementTypes)
{
	static std::unordered_map<size_t, UserType> cache;

	auto hash = std::accumulate(begin(elementTypes), end(elementTypes)
		, (14695981039346656037u ^ (size_t)prototype) * 1099511628211
		, [](auto accumulator, auto type)
		{
			return (accumulator ^ (size_t)type) * 1099511628211;
		});

	auto [iterator, _] = cache.try_emplace(hash, UserType { prototype, elementTypes });

	return &iterator->second;
}
