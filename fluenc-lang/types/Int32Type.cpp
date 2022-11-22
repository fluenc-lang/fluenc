#include "types/Int32Type.h"

std::string Int32Type::name() const
{
	return "i32";
}

llvm::Type *Int32Type::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt32Ty(context);
}
