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

	if (m_name == "bool")
	{
		return llvm::Type::getInt1Ty(*context);
	}

	if (m_name == "string")
	{
		return llvm::Type::getInt8PtrTy(*context);
	}

	return nullptr;
}

DzTypeName *DzTypeName::int32()
{
	static DzTypeName typeName("int");

	return &typeName;
}

DzTypeName *DzTypeName::int64()
{
	static DzTypeName typeName("long");

	return &typeName;
}

DzTypeName *DzTypeName::boolean()
{
	static DzTypeName typeName("bool");

	return &typeName;
}

DzTypeName *DzTypeName::string()
{
	static DzTypeName typeName("string");

	return &typeName;
}
