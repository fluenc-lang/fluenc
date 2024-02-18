#include "AnyType.h"

Type *any_type::instance()
{
	static any_type instance;

	return &instance;
}

type_id any_type::id() const
{
    return type_id::Any;
}

std::string any_type::name() const
{
	return "any";
}

llvm::Type *any_type::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt1Ty(context);
}
