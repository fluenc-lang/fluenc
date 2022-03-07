#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>

#include "DzMemberAccess.h"
#include "EntryPoint.h"
#include "IRBuilderEx.h"
#include "Type.h"

#include "values/TypedValue.h"
#include "values/LazyValue.h"
#include "values/ReferenceValue.h"
#include "values/ArrayValue.h"
#include "values/FunctionValue.h"

DzMemberAccess::DzMemberAccess(DzValue *consumer, const std::string &name)
	: m_consumer(consumer)
	, m_name(name)
{
}

std::vector<DzResult> DzMemberAccess::build(const EntryPoint &entryPoint, Stack values) const
{
	auto locals = entryPoint.locals();
	auto functions = entryPoint.functions();

	auto localsIterator = locals.find(m_name);

	if (localsIterator != locals.end())
	{
		if (auto value = dynamic_cast<const ReferenceValue *>(localsIterator->second))
		{
			auto valueType = value->type();

			IRBuilderEx builder(entryPoint);

			auto load = builder.createLoad(*value, m_name);

			values.push(new TypedValue { valueType, load });
		}
		else if (localsIterator->second)
		{
			values.push(localsIterator->second);
		}

		return m_consumer->build(entryPoint, values);
	}

	auto functionsIterator = functions.find(m_name);

	if (functionsIterator != functions.end())
	{
		auto value = new FunctionValue(functionsIterator->second, entryPoint);

		values.push(value);

		return m_consumer->build(entryPoint, values);
	}

	throw new std::exception();
}
