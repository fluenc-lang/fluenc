#include "int64_type.hpp"

namespace fluenc::code_generation::types
{
	std::string int64_type::name() const
	{
		return "i64";
	}

	llvm::Type* int64_type::storage_type(llvm::LLVMContext& context) const
	{
		return llvm::Type::getInt64Ty(context);
	}
}
