#include "Iteratable.h"
#include "utility.hpp"
#include "Iterator.h"
#include "Emitter.h"

Iteratable::Iteratable(const fragment *subject, const Type *type)
	: m_subject(subject)
	, m_type(type)
{
}

std::vector<DzResult> Iteratable::build(const Emitter &visitor, DefaultVisitorContext context) const
{
	auto llvmContext = context.entryPoint.context();

	auto block = createBlock(llvmContext);

	auto iteratorEntryPoint = context.entryPoint
		.withBlock(block);

	auto results = m_subject->build(visitor, { iteratorEntryPoint, context.values });

	auto iterator = new Iterator(m_subject, m_type, iteratorEntryPoint, Stack(), results);

	context.values.push(iterator);

	return {{ context.entryPoint.increaseDepth(), context.values }};
}
