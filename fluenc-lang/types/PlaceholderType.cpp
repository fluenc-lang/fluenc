#include "types/PlaceholderType.h"
#include "Utility.h"

TypeId PlaceholderType::id() const
{
	return TypeId::Placeholder;
}

std::string PlaceholderType::name() const
{
	return "__placeholder";
}

llvm::Type *PlaceholderType::storageType(llvm::LLVMContext &context) const
{
	UNUSED(context);

	return nullptr;
}
