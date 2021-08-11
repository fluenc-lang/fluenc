#include "DzMemberAccess.h"
#include "EntryPoint.h"

DzMemberAccess::DzMemberAccess(DzValue *consumer, const std::string &name)
	: m_consumer(consumer)
	, m_name(name)
{
}

std::vector<DzResult> DzMemberAccess::build(const EntryPoint &entryPoint, Stack values) const
{
	auto locals = entryPoint.locals();

	auto iterator = locals.find(m_name);

	if (iterator == locals.end())
	{
		return std::vector<DzResult>();
	}

	values.push(iterator->second);

	return m_consumer->build(entryPoint, values);
}
