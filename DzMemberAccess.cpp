#include "DzMemberAccess.h"
#include "EntryPoint.h"

DzMemberAccess::DzMemberAccess(DzValue *consumer, const std::string &name)
	: m_consumer(consumer)
	, m_name(name)
{
}

llvm::Value *DzMemberAccess::build(const EntryPoint &entryPoint, std::deque<llvm::Value *> &values) const
{
	auto locals = entryPoint.locals();

	auto iterator = locals.find(m_name);

	if (iterator == locals.end())
	{
		return nullptr;
	}

	values.push_back(iterator->second);

	return m_consumer->build(entryPoint, values);
}
