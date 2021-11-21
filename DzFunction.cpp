#include <map>
#include <algorithm>
#include <sstream>
#include <iostream>

#include <llvm/IR/Type.h>
#include <llvm/IR/IRBuilder.h>

#include "DzFunction.h"
#include "DzArgument.h"
#include "DzTypeName.h"
#include "EntryPoint.h"
#include "AllIterator.h"
#include "Type.h"
#include "IndexIterator.h"
#include "DzTupleArgument.h"

#include "types/UserType.h"

#include "values/ExpandableValue.h"
#include "values/TypedValue.h"
#include "values/TupleValue.h"
#include "values/BaseValue.h"
#include "values/UserTypeValue.h"
#include "values/NamedValue.h"

DzFunction::DzFunction(FunctionAttribute attribute
	, const std::string &name
	, const std::vector<DzBaseArgument *> &arguments
	, DzValue *block
	)
	: m_attribute(attribute)
	, m_name(name)
	, m_arguments(arguments)
	, m_block(block)
{
}

std::string DzFunction::name() const
{
	return m_name;
}

FunctionAttribute DzFunction::attribute() const
{
	return m_attribute;
}

bool DzFunction::hasMatchingSignature(const EntryPoint &entryPoint, const Stack &values) const
{
	if (m_arguments.size() != values.size())
	{
		return false;
	}

	auto result = true;

	std::transform(begin(m_arguments), end(m_arguments), values.rbegin(), all_true(result), [=](DzBaseArgument *argument, auto value)
	{
		if (!value)
		{
			return false;
		}

		auto argumentType = argument->type(entryPoint);
		auto valueType = value->type();

		return valueType->is(argumentType, entryPoint);
	});

	return result;
}

std::vector<DzResult> DzFunction::build(const EntryPoint &entryPoint, Stack values) const
{
	auto block = entryPoint.block();

	auto pep = entryPoint
		.withValues(values);

	auto locals = entryPoint.locals();

	for (const auto &argument : m_arguments)
	{
		for (auto &[name, value] : handleArgument(argument, entryPoint, values))
		{
			locals[name] = value;
		}
	}

	auto ep = pep
		.withName(m_name)
		.withEntry(block)
		.withLocals(locals);

	return m_block->build(ep, values);
}

std::vector<DzFunction::Argument> DzFunction::handleArgument(DzBaseArgument *argument, const EntryPoint &entryPoint, Stack &values) const
{
	if (auto standardArgument = dynamic_cast<DzArgument *>(argument))
	{
//		auto &module = entryPoint.module();
//		auto &context = entryPoint.context();

		auto name = standardArgument->name();

		auto value = values.pop();

		std::vector<Argument> result
		{
			{ name, value }
		};

		if (auto userValue = dynamic_cast<const UserTypeValue * >(value))
		{
			auto fields = userValue->fields();

			std::transform(begin(fields), end(fields), std::back_insert_iterator(result), [=](auto field) -> Argument
			{
				std::stringstream ss;
				ss << name;
				ss << ".";
				ss << field->name();

				auto localName = ss.str();

				return { localName, new Blast { field->subject(), field->entryPoint() } };
			});
		}

		return result;
	}

	if (auto tupleArgument = dynamic_cast<DzTupleArgument *>(argument))
	{
		auto tupleValue = values.require<TupleValue>();

		auto tupleValues = tupleValue->values();
		auto arguments = tupleArgument->arguments();

		std::vector<Argument> results;

		for (auto argument : arguments)
		{
			for (auto &result : handleArgument(argument, entryPoint, tupleValues))
			{
				results.push_back(result);
			}
		}

		return results;
	}

	throw new std::exception();
}
