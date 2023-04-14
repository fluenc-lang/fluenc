#include <llvm/IR/Type.h>

#include "WithoutType.h"
#include "WithoutOperatorSet.h"

TypeId WithoutType::id() const
{
	return TypeId::Without;
}

std::string WithoutType::name() const
{
	return "without";
}

llvm::Type *WithoutType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt1Ty(context);
}

IOperatorSet *WithoutType::operators() const
{
	static WithoutOperatorSet operators;

	return &operators;
}
