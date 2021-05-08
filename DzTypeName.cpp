#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>

#include "DzTypeName.h"
#include "EntryPoint.h"

DzTypeName::DzTypeName(std::string typeName)
	: m_typeName(typeName)
{
}

std::string DzTypeName::name() const
{
	return m_typeName;
}

llvm::Type *DzTypeName::type(const EntryPoint &entryPoint) const
{
	auto context = entryPoint.context();

	if (m_typeName == "int")
	{
		return llvm::Type::getInt32Ty(*context);
	}

	if (m_typeName == "consumer")
	{
		auto returnType = entryPoint.returnType();

		std::vector<llvm::Type *> argumentTypes = { llvm::Type::getInt32Ty(*context) };

		return llvm::PointerType::getUnqual(llvm::FunctionType::get(returnType, argumentTypes, false));
	}

	return nullptr;
}
