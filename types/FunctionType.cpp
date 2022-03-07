#include <sstream>

#include "FunctionType.h"
#include "AllIterator.h"

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

bool FunctionType::is(const Type *type, const EntryPoint &entryPoint) const
{
	if (auto other = dynamic_cast<const FunctionType *>(type))
	{
		if (m_types.size() != other->m_types.size())
		{
			return false;
		}

		auto result = true;

		std::transform(begin(m_types), end(m_types), begin(other->m_types), all_true(result), [&](auto left, auto right)
		{
			return left->is(right, entryPoint);
		});

		return result;
	}

	return false;
}

bool FunctionType::equals(const Type *type, const EntryPoint &entryPoint) const
{
	return is(type, entryPoint);
}
