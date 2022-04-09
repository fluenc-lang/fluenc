#include <map>
#include <algorithm>
#include <sstream>
#include <iostream>

#include <llvm/IR/Type.h>
#include <llvm/IR/IRBuilder.h>

#include "FunctionNode.h"
#include "DzArgument.h"
#include "DzTypeName.h"
#include "EntryPoint.h"
#include "AllIterator.h"
#include "Type.h"
#include "IndexIterator.h"
#include "DzTupleArgument.h"

#include "values/ExpandableValue.h"
#include "values/ScalarValue.h"
#include "values/TupleValue.h"
#include "values/BaseValue.h"
#include "values/UserTypeValue.h"
#include "values/NamedValue.h"

FunctionNode::FunctionNode(FunctionAttribute attribute
	, const std::string &name
	, const std::vector<DzBaseArgument *> &arguments
	, Node *block
	)
	: m_attribute(attribute)
	, m_name(name)
	, m_arguments(arguments)
	, m_block(block)
{
}

std::string FunctionNode::name() const
{
	return m_name;
}

std::vector<DzBaseArgument *> FunctionNode::arguments() const
{
	return m_arguments;
}

FunctionAttribute FunctionNode::attribute() const
{
	return m_attribute;
}

int8_t FunctionNode::signatureCompatibility(const EntryPoint &entryPoint, const Stack &values) const
{
	if (m_arguments.size() != values.size())
	{
		return -1;
	}

	std::vector<std::pair<const Type *, const Type *>> types;

	std::transform(begin(m_arguments), end(m_arguments), values.rbegin(), std::back_inserter(types), [=](DzBaseArgument *argument, auto value)
	{
		auto argumentType = argument->type(entryPoint);
		auto valueType = value->type();

		return std::make_pair(argumentType, valueType);
	});

	return std::accumulate(begin(types), end(types), 0, [=](auto score, auto pair) -> int8_t
	{
		if (score < 0)
		{
			return score;
		}

		auto [argumentType, valueType] = pair;

		auto compatibility = valueType->compatibility(argumentType, entryPoint);

		if (compatibility < 0)
		{
			return compatibility;
		}

		return score + compatibility;
	});
}

std::vector<DzResult> FunctionNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto pep = entryPoint
		.withValues(values);

	auto locals = entryPoint.locals();

	for (const auto &argument : m_arguments)
	{
		for (auto &[name, value] : handleArgument(argument, entryPoint, values.pop()))
		{
			locals[name] = value;
		}
	}

	auto ep = pep
		.withName(m_name)
		.markEntry()
		.withLocals(locals)
		.withIteratorStorage(nullptr);

	return m_block->build(ep, values);
}

std::vector<FunctionNode::Argument> FunctionNode::handleArgument(DzBaseArgument *argument, const EntryPoint &entryPoint, const BaseValue *value) const
{
	if (auto standardArgument = dynamic_cast<DzArgument *>(argument))
	{
		auto name = standardArgument->name();

		std::vector<Argument> result
		{
			{ name, value }
		};

		if (auto userValue = dynamic_cast<const UserTypeValue * >(value))
		{
			auto fields = userValue->fields();

			std::transform(begin(fields), end(fields), std::back_inserter(result), [=](auto field) -> Argument
			{
				std::stringstream ss;
				ss << name;
				ss << ".";
				ss << field->name();

				return { ss.str(), field->value() };
			});
		}

		return result;
	}

	if (auto tupleArgument = dynamic_cast<DzTupleArgument *>(argument))
	{
		auto tupleValue = dynamic_cast<const TupleValue *>(value);

		auto tupleValues = tupleValue->values();
		auto arguments = tupleArgument->arguments();

		std::vector<Argument> results;

		for (auto argument : arguments)
		{
			for (auto &result : handleArgument(argument, entryPoint, tupleValues.pop()))
			{
				results.push_back(result);
			}
		}

		return results;
	}

	throw new std::exception();
}
