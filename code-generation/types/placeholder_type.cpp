#include "placeholder_type.hpp"
#include "utility.hpp"

namespace fluenc::code_generation::types
{
	type_id placeholder_type::id() const
	{
		return type_id::placeholder;
	}

	std::string placeholder_type::name() const
	{
		return "__placeholder";
	}

	llvm::Type* placeholder_type::storage_type(llvm::LLVMContext& context) const
	{
		UNUSED(context);

		return nullptr;
	}
}
