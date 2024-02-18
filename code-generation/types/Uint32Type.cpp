#include "types/Uint32Type.h"

std::string uint32_type::name() const
{
	return "u32";
}

llvm::Type *uint32_type::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt32Ty(context);
}
