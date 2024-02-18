#include "VoidType.h"

type_id void_type::id() const
{
    return type_id::Void;
}

std::string void_type::name() const
{
	return "void";
}

llvm::Type *void_type::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getVoidTy(context);
}
