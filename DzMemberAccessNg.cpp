#include "DzMemberAccessNg.h"
#include "EntryPointInfo.h"

DzMemberAccessNg::DzMemberAccessNg(const std::string &name)
	: m_name(name)
{
}

llvm::Value *DzMemberAccessNg::build(const EntryPointInfo &entryPoint) const
{
	const auto &locals = entryPoint.locals();

	auto iterator = locals.find(m_name);

	if (iterator == locals.end())
	{
		return nullptr;
	}

	return iterator->second;
}
