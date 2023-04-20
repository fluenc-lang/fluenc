#include "types/IteratorType.h"

Type *IteratorType::instance()
{
	static IteratorType instance;

	return &instance;
}

TypeId IteratorType::id() const
{
	return TypeId::Iterator;
}

std::string IteratorType::name() const
{
	return "...";
}

llvm::Type *IteratorType::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt8PtrTy(context);
}
