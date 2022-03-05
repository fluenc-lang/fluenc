#include "AnyType.h"
#include "Utility.h"

std::string AnyType::name() const
{
	return "any";
}

llvm::Type *AnyType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt1Ty(context);
}

bool AnyType::is(const Type *type, const EntryPoint &entryPoint) const
{
	UNUSED(type);
	UNUSED(entryPoint);

	return true;
}

bool AnyType::equals(const Type *type, const EntryPoint &entryPoint) const
{
	UNUSED(type);
	UNUSED(entryPoint);

	return true;
}
