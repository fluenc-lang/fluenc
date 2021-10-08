#include "VoidType.h"
#include "Utility.h"

std::string VoidType::tag() const
{
	return "void";
}

llvm::Type *VoidType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getVoidTy(context);
}
