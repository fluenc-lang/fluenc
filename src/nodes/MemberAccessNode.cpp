#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>

#include "nodes/MemberAccessNode.h"
#include "EntryPoint.h"
#include "IRBuilderEx.h"
#include "Type.h"
#include "UndeclaredIdentifierException.h"

#include "values/ScalarValue.h"
#include "values/IteratorValue.h"
#include "values/ReferenceValue.h"
#include "values/ArrayValue.h"
#include "values/FunctionValue.h"

MemberAccessNode::MemberAccessNode(antlr4::ParserRuleContext *context, const Node *consumer, const std::vector<std::string> &names)
	: m_token(TokenInfo::fromContext(context))
	, m_consumer(consumer)
	, m_names(names)
{
}

std::vector<DzResult> MemberAccessNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto locals = entryPoint.locals();
	auto functions = entryPoint.functions();
	auto globals = entryPoint.globals();

	for (auto &name : m_names)
	{
		auto localsIterator = locals.find(name);

		if (localsIterator != locals.end())
		{
			if (auto value = dynamic_cast<const ReferenceValue *>(localsIterator->second))
			{
				IRBuilderEx builder(entryPoint);

				auto load = builder.createLoad(value, name);

				values.push(load);
			}
			else if (localsIterator->second)
			{
				values.push(localsIterator->second);
			}

			return m_consumer->build(entryPoint, values);
		}

		auto functionsIterator = functions.find(name);

		if (functionsIterator != functions.end())
		{
			auto value = new FunctionValue(functionsIterator->second, entryPoint);

			values.push(value);

			return m_consumer->build(entryPoint, values);
		}

		auto globalsIterator = globals.find(name);

		if (globalsIterator != globals.end())
		{
			std::vector<DzResult> results;

			for (auto &[resultEntryPoint, resultValues] : globalsIterator->second->build(entryPoint, values))
			{
				for (auto &result : m_consumer->build(resultEntryPoint, resultValues))
				{
					results.push_back(result);
				}
			}

			return results;
		}
	}

	throw new UndeclaredIdentifierException(m_token, m_names[0]);
}
