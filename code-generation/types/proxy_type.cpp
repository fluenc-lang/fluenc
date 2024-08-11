#include "proxy_type.hpp"
#include "utility.hpp"

namespace fluenc::code_generation::types
{
	proxy_type::proxy_type(llvm::Type* type)
		: type_(type)
	{
	}

	type_id proxy_type::id() const
	{
		return type_id::proxy;
	}

	std::string proxy_type::name() const
	{
		return "__proxy";
	}

	llvm::Type* proxy_type::storage_type(llvm::LLVMContext& context) const
	{
		UNUSED(context);

		return type_;
	}
}
