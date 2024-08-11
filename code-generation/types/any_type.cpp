#include "any_type.hpp"

namespace fluenc::code_generation::types
{
	base_type* any_type::instance()
	{
		static any_type instance;

		return &instance;
	}

	type_id any_type::id() const
	{
		return type_id::any;
	}

	std::string any_type::name() const
	{
		return "any";
	}

	llvm::Type* any_type::storage_type(llvm::LLVMContext& context) const
	{
		return llvm::Type::getInt1Ty(context);
	}
}
