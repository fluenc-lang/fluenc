#include "float32_type.hpp"

#include "types/float_operator_set.hpp"

namespace fluenc::code_generation::types
{
	std::string float32_type::name() const
	{
		return "f32";
	}

	llvm::Type* float32_type::storage_type(llvm::LLVMContext& context) const
	{
		return llvm::Type::getFloatTy(context);
	}

	operator_set* float32_type::operators() const
	{
		static float_operator_set operators;

		return &operators;
	}
}
