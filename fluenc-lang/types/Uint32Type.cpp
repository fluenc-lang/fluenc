#include "types/Uint32Type.h"

std::string Uint32Type::name() const
{
	return "u32";
}

llvm::Type *Uint32Type::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt32Ty(context);
}
