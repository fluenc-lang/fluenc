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

llvm::Type *AggregateType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt1Ty(context);
}

int8_t AggregateType::compatibility(const Type *type, const EntryPoint &entryPoint) const
{
	if (type == this)
	{
		return 0;
	}

	return std::accumulate(begin(m_subjects), end(m_subjects), static_cast<int8_t>(-1), [&](auto score, auto subject)
	{
		auto result = subject->compatibility(type, entryPoint);

		if (result < 0)
		{
			return score;
		}

		if (score < 0)
		{
			return result;
		}

		return std::min(result, score);
	});
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
