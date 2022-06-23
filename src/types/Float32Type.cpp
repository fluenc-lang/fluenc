#include "types/Float32Type.h"
#include "types/FloatOperatorSet.h"

std::string Float32Type::name() const
{
	return "f32";
}

llvm::Type *Float32Type::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getFloatTy(context);
}

IOperatorSet *Float32Type::operators() const
{
	static FloatOperatorSet operators;

	return &operators;
}
