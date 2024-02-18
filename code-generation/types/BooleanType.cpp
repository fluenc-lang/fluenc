#include "types/BooleanType.h"
#include "types/BooleanOperatorSet.h"

std::string boolean_type::name() const
{
	return "bool";
}

llvm::Type *boolean_type::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt1Ty(context);
}

IOperatorSet *boolean_type::operators() const
{
	static BooleanOperatorSet operators;

	return &operators;
}
