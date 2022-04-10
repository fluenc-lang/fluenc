#include <llvm/IR/Type.h>

#include "types/WithoutType.h"
#include "Utility.h"

std::string WithoutType::name() const
{
	return "without";
}

llvm::Type *WithoutType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt1Ty(context);
}
