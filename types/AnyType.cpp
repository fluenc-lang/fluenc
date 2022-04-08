#include "AnyType.h"
#include "Utility.h"

Type *AnyType::instance()
{
	static AnyType instance;

	return &instance;
}

std::string AnyType::name() const
{
	return "any";
}

llvm::Type *AnyType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt1Ty(context);
}

int8_t AnyType::compatibility(const Type *type, const EntryPoint &entryPoint) const
{
	UNUSED(type);
	UNUSED(entryPoint);

	return 1;
}
