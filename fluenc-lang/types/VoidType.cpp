#include "VoidType.h"

TypeId VoidType::id() const
{
	return TypeId::Void;
}

std::string VoidType::name() const
{
	return "void";
}

llvm::Type *VoidType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getVoidTy(context);
}
