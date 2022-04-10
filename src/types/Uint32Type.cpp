#include "types/Uint32Type.h"
#include "Utility.h"

std::string Uint32Type::name() const
{
	return "uint";
}

llvm::Type *Uint32Type::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt32Ty(context);
}
