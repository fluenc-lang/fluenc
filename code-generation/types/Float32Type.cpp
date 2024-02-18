#include "types/Float32Type.h"
#include "types/FloatOperatorSet.h"

std::string float32_type::name() const
{
	return "f32";
}

llvm::Type *float32_type::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getFloatTy(context);
}

IOperatorSet *float32_type::operators() const
{
	static FloatOperatorSet operators;

	return &operators;
}
