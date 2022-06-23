#include "types/BooleanType.h"
#include "types/BooleanOperatorSet.h"

#include "Utility.h"

std::string BooleanType::name() const
{
	return "bool";
}

llvm::Type *BooleanType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt1Ty(context);
}

IOperatorSet *BooleanType::operators() const
{
	static BooleanOperatorSet operators;

	return &operators;
}
