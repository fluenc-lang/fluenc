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

#include "types/UserType.h"

#include "values/ExpandableValue.h"
#include "values/TypedValue.h"
#include "values/TupleValue.h"
#include "values/BaseValue.h"

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
		if (auto standardArgument = dynamic_cast<DzArgument *>(argument))
		{
			for (auto &[name, value] : handleStandardArgument(standardArgument, entryPoint, values))
			{
				locals[name] = value;
			}
		}
		else if (auto tupleArgument = dynamic_cast<DzTupleArgument *>(argument))
		{
			auto k = (TupleValue *)values.pop();

			Stack s;

			for (auto &v : k->values())
			{
				s.push(v);
			}

			for (auto standardArgument : tupleArgument->arguments())
			{
				for (auto &[name, value] : handleStandardArgument(static_cast<DzArgument *>(standardArgument), entryPoint, s))
				{
					locals[name] = value;
				}
			}
		}
	}

	auto ep = pep
		.withName(m_name)
		.withEntry(block)
		.withLocals(locals);

	return m_block->build(ep, values);
}

std::vector<DzFunction::Argument> DzFunction::handleStandardArgument(DzArgument *argument, const EntryPoint &entryPoint, Stack &values) const
{
	auto &module = entryPoint.module();
	auto &context = entryPoint.context();

	auto name = argument->name();

	auto value = values.pop();

	std::vector<Argument> result =
	{
		{ name, value }
	};

	if (auto addressOfArgument = dynamic_cast<const TypedValue *>(value))
	{
		auto argumentType = addressOfArgument->type();

		if (auto userType = dynamic_cast<const UserType *>(argumentType))
		{
			auto block = entryPoint.block();

			auto dataLayout = module->getDataLayout();

			auto storageType = argumentType->storageType(*context);

			auto align = dataLayout.getABITypeAlign(storageType);

			auto intType = llvm::Type::getInt32Ty(*context);

			auto load = new llvm::LoadInst(storageType, *addressOfArgument, name, false, align, block);

			auto fields = userType->fields();

			std::transform(begin(fields), end(fields), index_iterator(), std::back_insert_iterator(result), [=](auto field, auto index) -> Argument
			{
				std::stringstream ss;
				ss << name;
				ss << ".";
				ss << field.name();

				auto localName = ss.str();

				auto fieldType = field.type();

				if (fieldType)
				{
					llvm::Value *indexes[] =
					{
						llvm::ConstantInt::get(intType, 0),
						llvm::ConstantInt::get(intType, index)
					};

					auto gep = llvm::GetElementPtrInst::CreateInBounds(load, indexes, field.name(), block);

					return { localName, new TypedValue(fieldType, gep) };
				}
				else
				{
					return { localName, nullptr };
				}
			});
		}
	}

	return result;
}
