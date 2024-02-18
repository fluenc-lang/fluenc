#include "types/Int32Type.h"

std::string int32_type::name() const
{
	return "i32";
}

llvm::Type *int32_type::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt32Ty(context);
}
