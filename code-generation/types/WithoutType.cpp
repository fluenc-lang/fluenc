#include <llvm/IR/Type.h>

#include "WithoutType.h"
#include "WithoutOperatorSet.h"

type_id without_type::id() const
{
    return type_id::Without;
}

std::string without_type::name() const
{
	return "without";
}

llvm::Type *without_type::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt1Ty(context);
}

IOperatorSet *without_type::operators() const
{
	static WithoutOperatorSet operators;

	return &operators;
}
