#include "BooleanType.h"
#include "Utility.h"

std::string BooleanType::name() const
{
	return "bool";
}

llvm::Type *BooleanType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt1Ty(context);
}
