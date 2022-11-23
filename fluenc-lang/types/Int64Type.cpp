#include "types/Int64Type.h"

std::string Int64Type::name() const
{
	return "i64";
}

llvm::Type *Int64Type::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt64Ty(context);
}
