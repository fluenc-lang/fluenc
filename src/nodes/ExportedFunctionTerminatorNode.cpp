#include "nodes/ExportedFunctionTerminatorNode.h"

std::vector<DzResult<BaseValue>> ExportedFunctionTerminatorNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitExportedFunctionTerminator(this, entryPoint, values);
}

std::vector<DzResult<BaseValue>> ExportedFunctionTerminatorNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitExportedFunctionTerminator(this, entryPoint, values);
}
