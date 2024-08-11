#include "boolean_type.hpp"

#include "types/boolean_operator_set.hpp"

namespace fluenc::code_generation::types
{
	std::string boolean_type::name() const
	{
		return "bool";
	}

	llvm::Type* boolean_type::storage_type(llvm::LLVMContext& context) const
	{
		return llvm::Type::getInt1Ty(context);
	}

	operator_set* boolean_type::operators() const
	{
		static boolean_operator_set operators;

		return &operators;
	}
}
