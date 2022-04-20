#include "Utility.h"

#include "types/StringType.h"

StringType::StringType()
	: m_length(-1)
{
}

StringType::StringType(size_t length)
	: m_length(length)
{
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

StringType *StringType::get(size_t length)
{
	static std::unordered_map<size_t, StringType> cache;

	auto [iterator, _] = cache.try_emplace(length, length);

	return &iterator->second;
}
