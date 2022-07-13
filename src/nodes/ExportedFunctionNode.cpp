#include "nodes/ExportedFunctionNode.h"

ExportedFunctionNode::ExportedFunctionNode(const std::string &name
	, Node *block
	, ITypeName *returnType
	)
	: m_name(name)
	, m_block(block)
	, m_returnType(returnType)
{
}

std::string ExportedFunctionNode::name() const
{
	return m_name;
}

std::vector<DzBaseArgument *> ExportedFunctionNode::arguments() const
{
	return std::vector<DzBaseArgument *>();
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

std::vector<DzResult<BaseValue>> ExportedFunctionNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitExportedFunction(this, entryPoint, values);
}

std::vector<DzResult<BaseValue> > ExportedFunctionNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitExportedFunction(this, entryPoint, values);
}
