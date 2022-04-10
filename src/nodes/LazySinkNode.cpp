#include "nodes/LazySinkNode.h"

#include "values/LazyValue.h"
#include "values/IteratorValueGenerator.h"

LazySinkNode::LazySinkNode(const Type *iteratorType
	, const Node *consumer
	, const Node *subject
	)
	: m_iteratorType(iteratorType)
	, m_consumer(consumer)
	, m_subject(subject)
{
}

std::vector<DzResult> LazySinkNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto generator = new IteratorValueGenerator(m_iteratorType, m_subject, entryPoint);
	auto lazy = new LazyValue(generator);

	values.push(lazy);

	return m_consumer->build(entryPoint, values);
}
