#include "BooleanType.h"

std::string BooleanType::tag() const
{
	return "bool";
}

llvm::Type *BooleanType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt1Ty(context);
}
