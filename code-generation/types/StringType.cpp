#include <unordered_map>

#include "types/StringType.h"
#include "types/StringOperatorSet.h"

string_type::string_type()
	: m_length(-1)
{
}

string_type::string_type(size_t length)
	: m_length(length)
{
}

type_id string_type::id() const
{
    return type_id::String;
}

size_t string_type::length() const
{
	return m_length;
}

std::string string_type::name() const
{
	return "string";
}

llvm::Type *string_type::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt8PtrTy(context);
}

IOperatorSet *string_type::operators() const
{
	static StringOperatorSet operators;

	return &operators;
}
