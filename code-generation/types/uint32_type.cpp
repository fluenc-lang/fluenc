#include "uint32_type.hpp"

namespace fluenc::code_generation::types
{
	std::string uint32_type::name() const
	{
		return "u32";
	}

	llvm::Type* uint32_type::storage_type(llvm::LLVMContext& context) const
	{
		return llvm::Type::getInt32Ty(context);
	}
}
