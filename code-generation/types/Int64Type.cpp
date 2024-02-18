#include "types/Int64Type.h"

std::string int64_type::name() const
{
	return "i64";
}

llvm::Type *int64_type::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt64Ty(context);
}
