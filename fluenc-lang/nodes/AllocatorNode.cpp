#include <numeric>

#include <range/v3/view.hpp>

#include "AllocatorNode.h"
#include "ArraySinkNode.h"
#include "IndexIterator.h"
#include "IndexSinkNode.h"
#include "Indexed.h"
#include "TerminatorNode.h"
#include "BaseValue.h"

#include "types/ArrayType.h"
#include "types/StringType.h"
#include "types/UserType.h"
#include "types/IPrototype.h"
#include "types/WithoutType.h"
#include "types/Int64Type.h"

#include "values/NamedValue.h"
#include "values/ScalarValue.h"
#include "values/UserTypeValue.h"
#include "values/LazyValue.h"
#include "values/WithoutValue.h"
#include "values/ReferenceValue.h"
#include "values/StringValue.h"
#include "values/BufferValue.h"

AllocatorNode::AllocatorNode(const Type *type, const Node *consumer)
	: m_type(type)
	, m_consumer(consumer)
{
}

AllocatorNode::AllocResult AllocatorNode::alloc(const Type *type, const DefaultNodeVisitor &visitor, const EntryPoint &entryPoint)
{
	if (auto userType = type_cast<const UserType *>(type))
	{
		auto elementTypes = userType->elementTypes();
		auto prototype = userType->prototype();

		auto [fieldsEntryPoint, fields] = prototype->fields(entryPoint, visitor);

		std::vector<const NamedValue *> values;

		auto zipped = ranges::views::zip(fields, elementTypes);

		auto accumulatedEntryPoint = std::accumulate(begin(zipped), end(zipped), fieldsEntryPoint, [&](auto currentEntryPoint, auto pair)
		{
			auto &[field, elementType] = pair;

			auto [allocEntryPoint, allocValue] = alloc(elementType, visitor, currentEntryPoint);

			auto namedValue = new NamedValue(field.name(), allocValue);

			values.push_back(namedValue);

			return allocEntryPoint;
		});

		return { accumulatedEntryPoint, new UserTypeValue(userType->prototype(), values) };
	}

	if (auto array = type_cast<const ArrayType *>(type))
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
			return { result.entryPoint, result.values.require<LazyValue>(nullptr) };
		}
	}

	if (auto string = type_cast<const StringType *>(type))
	{
		auto alloc = entryPoint.alloc(string);
		auto length = entryPoint.alloc(Int64Type::instance());

		return { entryPoint, new StringValue(alloc, length) };
	}

	if (auto buffer = type_cast<const BufferType *>(type))
	{
		auto alloc = entryPoint.alloc(type);

		return { entryPoint, new BufferValue(alloc) };
	}

	if (type->id() == TypeId::Without)
	{
		return { entryPoint, WithoutValue::instance() };
	}

	return { entryPoint, entryPoint.alloc(type) };
}

std::vector<DzResult> AllocatorNode::accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const
{
	auto [entryPoint, value] = alloc(m_type, visitor, context.entryPoint);

	context.values.push(value);

	return m_consumer->accept(visitor, { entryPoint, context.values });
}
