#include <llvm/IR/Type.h>

#include "without_operator_set.hpp"
#include "without_type.hpp"

namespace fluenc::code_generation::types
{
	type_id without_type::id() const
	{
		return type_id::without;
	}

	std::string without_type::name() const
	{
		return "without";
	}

	llvm::Type* without_type::storage_type(llvm::LLVMContext& context) const
	{
		return llvm::Type::getInt1Ty(context);
	}

	operator_set* without_type::operators() const
	{
		static without_operator_set operators;

		return &operators;
	}
}
