#include <sstream>

#include "FunctionType.h"
#include "AllIterator.h"

#include "iterators/ExtremitiesIterator.h"

FunctionType::FunctionType(const std::vector<const Type *> &types)
	: m_types(types)
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

	return max;
}
