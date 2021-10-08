#include "StringType.h"
#include "Utility.h"

std::string StringType::tag() const
{
	return "string";
}

llvm::Type *StringType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt8PtrTy(context);
}
