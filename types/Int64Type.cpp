#include "Int64Type.h"
#include "Utility.h"

std::string Int64Type::tag() const
{
	return "long";
}

llvm::Type *Int64Type::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt64Ty(context);
}
