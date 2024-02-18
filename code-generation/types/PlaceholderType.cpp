#include "types/PlaceholderType.h"
#include "utility.hpp"

type_id placeholder_type::id() const
{
    return type_id::Placeholder;
}

std::string placeholder_type::name() const
{
	return "__placeholder";
}

llvm::Type *placeholder_type::storageType(llvm::LLVMContext &context) const
{
	UNUSED(context);

	return nullptr;
}
