#include "buffer_type.hpp"

namespace fluenc::code_generation::types
{
	type_id buffer_type::id() const
	{
		return type_id::buffer;
	}

	std::string buffer_type::name() const
	{
		return "buffer";
	}

	llvm::Type* buffer_type::storage_type(llvm::LLVMContext& context) const
	{
		return llvm::Type::getInt8PtrTy(context);
	}
}
