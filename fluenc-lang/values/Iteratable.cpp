#include "Iteratable.h"
#include "Node.h"
#include "Utility.h"
#include "Iterator.h"

#include "nodes/IteratorAdapter.h"

Iteratable::Iteratable(const IIteratable *subject, const Type *type)
	: m_subject(subject)
	, m_type(type)
{

}
std::vector<DzResult> Iteratable::accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const
{
	auto llvmContext = context.entryPoint.context();

	auto block = createBlock(llvmContext);

	auto iteratorEntryPoint = context.entryPoint
		.withBlock(block);

	auto results = m_subject->accept(visitor, { iteratorEntryPoint, context.values });

	auto adapter = new IteratorAdapter(m_subject);
	auto iterator = new Iterator(adapter, m_type, iteratorEntryPoint, Stack(), results);

	context.values.push(iterator);

	return {{ context.entryPoint, context.values }};
}
