#include "VoidType.h"
#include "Utility.h"

std::string VoidType::name() const
{
	return "void";
}

llvm::Type *VoidType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getVoidTy(context);
}
