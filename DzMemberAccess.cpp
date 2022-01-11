#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>

#include "DzMemberAccess.h"
#include "EntryPoint.h"
#include "IRBuilderEx.h"
#include "Type.h"

#include "values/TypedValue.h"
#include "values/LazyValue.h"
#include "values/ReferenceValue.h"

DzMemberAccess::DzMemberAccess(DzValue *consumer, const std::string &name)
	: m_consumer(consumer)
	, m_name(name)
{
}

std::vector<DzResult> DzMemberAccess::build(const EntryPoint &entryPoint, Stack values) const
{
	auto block = entryPoint.block();
	auto locals = entryPoint.locals();

	auto iterator = locals.find(m_name);

	if (iterator == locals.end())
	{
		throw new std::exception();
	}

	if (auto value = dynamic_cast<const ReferenceValue *>(iterator->second))
	{
		auto valueType = value->type();

		IRBuilderEx builder(entryPoint);

		auto load = builder.createLoad(*value, m_name);

		values.push(new TypedValue { valueType, load });
	}
	else if (auto value = dynamic_cast<const LazyValue *>(iterator->second))
	{
		std::vector<DzResult> results;

		for (auto &[forwardedEntryPoint, forwardedValues] : value->build(block, values))
		{
			auto consumerEntryPoint = entryPoint
				.withBlock(forwardedEntryPoint.block());

			for (auto &result : m_consumer->build(consumerEntryPoint, forwardedValues))
			{
				results.push_back(result);
			}
		}

		return results;
	}
	else if (iterator->second)
	{
		values.push(iterator->second);
	}

	return m_consumer->build(entryPoint, values);
}
