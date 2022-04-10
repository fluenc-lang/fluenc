#include "types/StringType.h"
#include "Utility.h"

std::string StringType::name() const
{
	return "string";
}

llvm::Type *StringType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt8PtrTy(context);
}
