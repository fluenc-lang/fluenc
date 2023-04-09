#include "Iterator.h"
#include "Utility.h"
#include "Node.h"

Iterator::Iterator(const Node *node, const Type *type, const EntryPoint &entryPoint, const Stack &values, const std::vector<DzResult> &results)
	: m_node(node)
	, m_type(type)
	, m_entryPoint(new EntryPoint(entryPoint))
	, m_values(values)
	, m_results(results)
	, m_strategies({ &Iterator::useRendered, &Iterator::createNew })
{
}

const Type *Iterator::type() const
{
	return m_type;
}

const BaseValue *Iterator::clone(const EntryPoint &entryPoint, CloneStrategy strategy) const
{
	UNUSED(entryPoint);
	UNUSED(strategy);

	return this;
}

std::vector<DzResult> Iterator::generate(const DefaultNodeVisitor &emitter, DefaultVisitorContext context) const
{
	auto strategy = m_strategies.get();

	return (this->*strategy)(emitter, context);
}

std::vector<DzResult> Iterator::useRendered(const DefaultNodeVisitor &emitter, DefaultVisitorContext context) const
{
	UNUSED(emitter);

	linkBlocks(context.entryPoint.block(), m_entryPoint->block());

	std::vector<DzResult> results;

	for (auto &[resultEntryPoint, resultValues] : m_results)
	{
		auto forwardedEntryPoint = resultEntryPoint
			.withLocals(context.entryPoint.locals());

		results.push_back({ forwardedEntryPoint, resultValues });
	}

	return results;
}

std::vector<DzResult> Iterator::createNew(const DefaultNodeVisitor &emitter, DefaultVisitorContext context) const
{
	return m_node->accept(emitter, { context.entryPoint, m_values });
}
