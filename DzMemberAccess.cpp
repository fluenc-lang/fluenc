#include <llvm/IR/IRBuilder.h>

#include "DzMemberAccess.h"
#include "EntryPoint.h"
#include "UndeclaredIdentifierException.h"

DzMemberAccess::DzMemberAccess(std::string name)
	: m_name(name)
{
}

llvm::Value *DzMemberAccess::build(const EntryPoint &context) const
{
	auto locals = context.locals();
	auto result = locals.find(m_name);

	if (result == locals.end())
	{
		return nullptr;
	}

	std::cout << m_name;

	return result->second->build(context);
}
