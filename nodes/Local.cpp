#include "Local.h"

Local::Local(const Node *consumer, const std::string &name)
	: m_consumer(consumer)
	, m_name(name)
{
}

std::vector<DzResult> Local::build(const EntryPoint &entryPoint, Stack values) const
{
	auto locals = entryPoint.locals();

	locals[m_name] = values.pop();

	auto ep = entryPoint
		.withLocals(locals);

	return m_consumer->build(ep, values);
}
