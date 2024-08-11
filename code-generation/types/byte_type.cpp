#include "byte_type.hpp"

namespace fluenc::code_generation::types
{
	std::string byte_type::name() const
	{
		return "u8";
	}

	llvm::Type* byte_type::storage_type(llvm::LLVMContext& context) const
	{
		return llvm::Type::getInt8Ty(context);
	}
}
