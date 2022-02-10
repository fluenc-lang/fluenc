#include <sstream>
#include <numeric>

#include "UserType.h"
#include "Utility.h"

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

bool UserType::is(const Type *type, const EntryPoint &entryPoint) const
{
	return m_prototype->is(type, entryPoint);
}

bool UserType::equals(const Type *type, const EntryPoint &entryPoint) const
{
	auto out = dynamic_cast<const UserType *>(type);

	if (!out)
	{
		return false;
	}

	if (out->m_elementTypes.size() != m_elementTypes.size())
	{
		return false;
	}

	for (auto i = 0u; i < m_elementTypes.size(); i++)
	{
		if (!m_elementTypes[i]->equals(out->m_elementTypes[i], entryPoint) &&
			!out->m_elementTypes[i]->equals(m_elementTypes[i], entryPoint))
		{
			return false;
		}
	}

	return true;
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
