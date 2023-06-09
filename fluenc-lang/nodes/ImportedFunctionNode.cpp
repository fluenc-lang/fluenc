#include "DzArgument.h"
#include "Type.h"
#include "TypeCompatibilityCalculator.h"

#include "nodes/ImportedFunctionNode.h"

#include "iterators/ExtremitiesIterator.h"

ImportedFunctionNode::ImportedFunctionNode(ITypeName *returnType
	, const std::string &name
	, const std::shared_ptr<peg::Ast> ast
	, const std::vector<DzBaseArgument *> &arguments
	)
	: m_returnType(returnType)
	, m_name(name)
	, m_ast(ast)
	, m_arguments(arguments)
{
}

ITypeName *ImportedFunctionNode::returnType() const
{
	return m_returnType;
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

int8_t ImportedFunctionNode::signatureCompatibility(const EntryPoint &entryPoint, const std::vector<const Type *> &values) const
{
	if (m_arguments.size() != values.size())
	{
		return -1;
	}

	int8_t min = 0;
	int8_t max = 0;

	std::transform(begin(m_arguments), end(m_arguments), begin(values), extremities_iterator(min, max), [=](auto argument, auto valueType) -> int8_t
	{
		if (!valueType)
		{
			return -1;
		}

		auto argumentType = argument->type(entryPoint);

		return TypeCompatibilityCalculator::calculate(entryPoint, valueType, argumentType);
	});

	if (min < 0)
	{
		return min;
	}

	return max;
}

std::vector<DzResult > ImportedFunctionNode::accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const
{
	return visitor.visit(this, context);
}
