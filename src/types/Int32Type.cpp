#include "Int32Type.h"
#include "Utility.h"

std::string Int32Type::name() const
{
	return "int";
}

llvm::Type *Int32Type::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt32Ty(context);
}
