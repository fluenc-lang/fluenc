#include <unordered_map>

#include "types/AggregateType.h"

AggregateType::AggregateType(const std::vector<const Type *> &subjects)
	: m_subjects(subjects)
{
}

std::string AggregateType::name() const
{
	std::ostringstream stream;

	stream << "<";

	for (auto i = begin(m_subjects); i != end(m_subjects); i++)
	{
		if (i != begin(m_subjects))
		{
			stream << ", ";
		}

		stream << (*i)->name();
	}

	stream << ">";

	return stream.str();
}

std::string AggregateType::fullName() const
{
	std::ostringstream stream;

	stream << "<";

	for (auto i = begin(m_subjects); i != end(m_subjects); i++)
	{
		if (i != begin(m_subjects))
		{
			stream << ", ";
		}

		stream << (*i)->fullName();
	}

	stream << ">";

	return stream.str();
}

std::vector<const Type *> AggregateType::subjects() const
{
	return m_subjects;
}

llvm::Type *AggregateType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt1Ty(context);
}

AggregateType *AggregateType::get(const std::vector<const Type *> &types)
{
	static std::unordered_map<size_t, AggregateType> cache;

	auto hash = std::accumulate(begin(types), end(types), 14695981039346656037u, [](auto accumulator, auto type)
	{
		return (accumulator ^ (size_t)type) * 1099511628211;
	});

	auto [iterator, _] = cache.try_emplace(hash, types);

	return &iterator->second;
}
