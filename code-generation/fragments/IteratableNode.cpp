#include "IteratableNode.h"
#include "IRBuilderEx.h"
#include "Emitter.h"

#include "values/ScalarValue.h"
#include "values/ReferenceValue.h"

#include "types/Int64Type.h"

IteratableNode::IteratableNode(const fragment *iteratable)
	: m_subject(iteratable)
{
}

std::vector<DzResult> IteratableNode::build(const Emitter &visitor, DefaultVisitorContext context) const
{
    auto index = context.values.require<scalar_value>(nullptr);
        auto alloc = context.entryPoint.alloc(int64_type::instance());

	IRBuilderEx builder(context.entryPoint);

	builder.createStore(index, alloc);

	context.values.push(alloc);

	return m_subject->build(visitor, context);
}
