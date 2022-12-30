#include "Utility.h"

#include "nodes/ExportedFunctionNode.h"

ExportedFunctionNode::ExportedFunctionNode(const std::string &name
	, const std::vector<DzBaseArgument *> &arguments
	, Node *block
	, ITypeName *returnType
	)
	: m_name(name)
	, m_arguments(arguments)
	, m_block(block)
	, m_returnType(returnType)
{
}

ITypeName *ExportedFunctionNode::returnType() const
{
	return m_returnType;
}

std::string ExportedFunctionNode::name() const
{
	return m_name;
}

std::vector<DzBaseArgument *> ExportedFunctionNode::arguments() const
{
	return m_arguments;
}

FunctionAttribute ExportedFunctionNode::attribute() const
{
	return FunctionAttribute::Export;
}

int8_t ExportedFunctionNode::signatureCompatibility(const EntryPoint &entryPoint, const std::vector<const Type *> &values) const
{
	UNUSED(entryPoint);
	UNUSED(values);

	return 0;
}

std::vector<DzResult> ExportedFunctionNode::accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const
{
	return visitor.visit(this, context);
}
