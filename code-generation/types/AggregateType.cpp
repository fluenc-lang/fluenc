#include <unordered_map>
#include <sstream>
#include <numeric>

#include "types/AggregateType.h"

aggregate_type::aggregate_type(const std::vector<const Type *> &subjects)
	: m_subjects(subjects)
{
}

type_id aggregate_type::id() const
{
    return type_id::Aggregate;
}

std::string aggregate_type::name() const
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

std::string aggregate_type::fullName() const
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

std::vector<const Type *> aggregate_type::subjects() const
{
	return m_subjects;
}

llvm::Type *aggregate_type::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt1Ty(context);
}

aggregate_type *aggregate_type::get(const std::vector<const Type *> &types)
{
	static std::unordered_map<size_t, aggregate_type> cache;

	auto hash = std::accumulate(begin(types), end(types), 14695981039346656037u, [](auto accumulator, auto type)
	{
		return (accumulator ^ (size_t)type) * 1099511628211;
	});

	auto [iterator, _] = cache.try_emplace(hash, types);

	return &iterator->second;
}
