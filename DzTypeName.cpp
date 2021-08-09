#include "DzTypeName.h"
#include "EntryPoint.h"

DzTypeName::DzTypeName(const std::string &name)
	: m_name(name)
{
}

std::string DzTypeName::name() const
{
	return m_name;
}

llvm::Type *DzTypeName::resolve(const EntryPoint &entryPoint)
{
	auto &context = entryPoint.context();

	if (m_name == "int")
	{
		return llvm::Type::getInt32Ty(*context);
	}

	if (m_name == "long")
	{
		return llvm::Type::getInt64Ty(*context);
	}

	return nullptr;
}
