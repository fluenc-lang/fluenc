#include "int32_type.hpp"

namespace fluenc::code_generation::types
{
	std::string int32_type::name() const
	{
		return "i32";
	}

	llvm::Type* int32_type::storage_type(llvm::LLVMContext& context) const
	{
		return llvm::Type::getInt32Ty(context);
	}
}
