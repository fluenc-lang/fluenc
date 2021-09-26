#include "Int32Type.h"

std::string Int32Type::tag() const
{
	return "int";
}

llvm::Type *Int32Type::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt32Ty(context);
}
