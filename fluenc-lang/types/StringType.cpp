#include <unordered_map>

#include "types/StringType.h"
#include "types/StringOperatorSet.h"

StringType::StringType()
	: m_length(-1)
{
}

StringType::StringType(size_t length)
	: m_length(length)
{
}

TypeId StringType::id() const
{
	return TypeId::String;
}

size_t StringType::length() const
{
	return m_length;
}

std::string StringType::name() const
{
	return "string";
}

llvm::Type *StringType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt8PtrTy(context);
}

IOperatorSet *StringType::operators() const
{
	static StringOperatorSet operators;

	return &operators;
}
