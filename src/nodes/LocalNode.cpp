#include <sstream>
#include <numeric>

#include "nodes/LocalNode.h"

#include "values/UserTypeValue.h"
#include "values/NamedValue.h"

LocalNode::LocalNode(const Node *consumer, const std::string &name)
	: m_consumer(consumer)
	, m_name(name)
{
}

std::vector<DzResult> LocalNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto locals = entryPoint.locals();

	auto value = values.pop();

	if (auto userValue = dynamic_cast<const UserTypeValue * >(value))
	{
		auto fields = userValue->fields();

		std::transform(begin(fields), end(fields), std::inserter(locals, begin(locals)), [=](auto field) -> std::pair<std::string, const BaseValue *>
		{
			std::stringstream ss;
			ss << m_name;
			ss << ".";
			ss << field->name();

			return { ss.str(), field->value() };
		});
	}

	locals[m_name] = value;

	auto ep = entryPoint
		.withLocals(locals);

	return m_consumer->build(ep, values);
}
