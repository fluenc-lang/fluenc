#include "types/AnyType.h"
#include "Utility.h"

Type *AnyType::instance()
{
	static AnyType instance;

	return &instance;
}

TypeId AnyType::id() const
{
	return TypeId::Any;
}

std::string AnyType::name() const
{
	return "any";
}

llvm::Type *AnyType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt1Ty(context);
}
