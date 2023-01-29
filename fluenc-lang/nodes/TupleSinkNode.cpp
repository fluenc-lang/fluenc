#include "TupleSinkNode.h"

#include "values/TupleValue.h"

TupleSinkNode::TupleSinkNode(size_t size, const Node *consumer)
	: m_size(size)
	, m_consumer(consumer)
{

}

std::vector<DzResult> TupleSinkNode::accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const
{
	std::vector<const BaseValue *> values;

	for (auto i = 0u; i < m_size; i++)
	{
		auto value = context.values.pop();

		values.push_back(value);
	}

	auto tuple = new TupleValue(values);

	context.values.push(tuple);

	return m_consumer->accept(visitor, context);
}
