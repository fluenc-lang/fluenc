#include <llvm/IR/Type.h>

#include "WithoutType.h"
#include "Utility.h"

std::string WithoutType::tag() const
{
	return "without";
}

llvm::Type *WithoutType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt1Ty(context);
}
