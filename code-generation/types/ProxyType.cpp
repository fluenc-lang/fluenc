#include "types/ProxyType.h"
#include "utility.hpp"

proxy_type::proxy_type(llvm::Type *type)
	: m_type(type)
{
}

type_id proxy_type::id() const
{
    return type_id::Proxy;
}

std::string proxy_type::name() const
{
	return "__proxy";
}

llvm::Type *proxy_type::storageType(llvm::LLVMContext &context) const
{
	UNUSED(context);

	return m_type;
}
