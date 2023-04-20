#include "types/ProxyType.h"
#include "Utility.h"

ProxyType::ProxyType(llvm::Type *type)
	: m_type(type)
{
}

TypeId ProxyType::id() const
{
	return TypeId::Proxy;
}

std::string ProxyType::name() const
{
	return "__proxy";
}

llvm::Type *ProxyType::storageType(llvm::LLVMContext &context) const
{
	UNUSED(context);

	return m_type;
}
