#include "DzTypeNameNg.h"
#include "EntryPointInfo.h"

DzTypeNameNg::DzTypeNameNg(const std::string &name)
	: m_name(name)
{
}

llvm::Type *DzTypeNameNg::build(const EntryPointInfo &entryPoint) const
{
	auto &context = entryPoint.context();

	if (m_name == "int")
	{
		return llvm::Type::getInt32Ty(*context);
	}

	return nullptr;
}
