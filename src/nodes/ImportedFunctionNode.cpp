#include "DzArgument.h"

#include "nodes/ImportedFunctionNode.h"

#include "iterators/ExtremitiesIterator.h"

ImportedFunctionNode::ImportedFunctionNode(ITypeName *returnType
	, const std::string &name
	, const std::shared_ptr<peg::Ast> &ast
	, const std::vector<DzBaseArgument *> &arguments
	)
	: m_returnType(returnType)
	, m_name(name)
	, m_ast(ast)
	, m_arguments(arguments)
{
}

std::string ImportedFunctionNode::name() const
{
	return m_name;
}

std::vector<DzBaseArgument *> ImportedFunctionNode::arguments() const
{
	return m_arguments;
}

FunctionAttribute ImportedFunctionNode::attribute() const
{
	return FunctionAttribute::Import;
}

int8_t ImportedFunctionNode::signatureCompatibility(const EntryPoint &entryPoint, const Stack &values) const
{
	if (m_arguments.size() != values.size())
	{
		return -1;
	}

	int8_t min = 0;
	int8_t max = 0;

	std::transform(begin(m_arguments), end(m_arguments), values.rbegin(), extremities_iterator(min, max), [=](auto argument, auto value) -> int8_t
	{
		if (!value)
		{
			return -1;
		}

		auto argumentType = argument->type(entryPoint);
		auto valueType = value->type();

		return valueType->compatibility(argumentType, entryPoint);
	});

	if (min < 0)
	{
		return min;
	}

	return max;
}

std::vector<DzResult> ImportedFunctionNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitImportedFunction(this, entryPoint, values);
}
