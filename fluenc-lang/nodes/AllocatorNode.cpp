#include <numeric>

#include "AllocatorNode.h"
#include "ArraySinkNode.h"
#include "IndexIterator.h"
#include "IndexSinkNode.h"
#include "Indexed.h"
#include "TerminatorNode.h"
#include "BaseValue.h"

#include "types/ArrayType.h"
#include "types/UserType.h"
#include "types/IPrototype.h"
#include "types/WithoutType.h"

#include "values/NamedValue.h"
#include "values/UserTypeValue.h"
#include "values/LazyValue.h"
#include "values/WithoutValue.h"
#include "values/ReferenceValue.h"

AllocatorNode::AllocatorNode(const Type *type, const Node *consumer)
	: m_type(type)
	, m_consumer(consumer)
{
}

const BaseValue *AllocatorNode::alloc(const Type *type, const DefaultNodeVisitor &visitor, const EntryPoint &entryPoint)
{
	if (auto userType = dynamic_cast<const UserType *>(type))
	{
		auto elementTypes = userType->elementTypes();
		auto prototype = userType->prototype();

		auto fields = prototype->fields(entryPoint, visitor);

		std::vector<const NamedValue *> values;

		transform(begin(fields), end(fields), rbegin(elementTypes), back_inserter(values), [&](auto field, auto elementType)
		{
			return new NamedValue(field.name()
				, alloc(elementType, visitor, entryPoint)
				);
		});

		return new UserTypeValue(userType->prototype(), values);
	}

	if (auto array = dynamic_cast<const ArrayType *>(type))
	{
		auto types = array->types();

		std::vector<Indexed<const Type *>> indexed;

		transform(begin(types), end(types), index_iterator(0), back_inserter(indexed), [&](auto type, auto index) -> Indexed<const Type *>
		{
			return { index, type };
		});

		auto firstValue = std::accumulate(begin(indexed), end(indexed), static_cast<Node *>(TerminatorNode::instance()), [&](auto next, auto pair)
		{
			auto indexSink = new IndexSinkNode(pair.index, next);
			auto value = new AllocatorNode(pair.value, indexSink);

			return value;
		});

		auto sink = new ArraySinkNode(types.size(), {}, TerminatorNode::instance(), firstValue);

		for (auto &result : sink->accept(visitor, { entryPoint, Stack() }))
		{
			return result.values.require<LazyValue>(nullptr);
		}
	}

	if (dynamic_cast<const WithoutType *>(type))
	{
		return WithoutValue::instance();
	}

	return entryPoint.alloc(type);
}

std::vector<DzResult> AllocatorNode::accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const
{
	auto value = alloc(m_type, visitor, context.entryPoint);

	context.values.push(value);

	return m_consumer->accept(visitor, context);
}
