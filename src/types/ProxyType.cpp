#include "types/ProxyType.h"
#include "Utility.h"

ProxyType::ProxyType(llvm::Type *type)
	: m_type(type)
{
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

int8_t ProxyType::compatibility(const Type *type, const EntryPoint &entryPoint) const
{
	UNUSED(type);
	UNUSED(entryPoint);

	return -1;
}
