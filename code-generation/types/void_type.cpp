#include "void_type.hpp"

namespace fluenc::code_generation::types
{
	type_id void_interop_type::id() const
	{
		return type_id::void_interop;
	}

	std::string void_interop_type::name() const
	{
		return "void";
	}

	llvm::Type* void_interop_type::storage_type(llvm::LLVMContext& context) const
	{
		return llvm::Type::getVoidTy(context);
	}
}
