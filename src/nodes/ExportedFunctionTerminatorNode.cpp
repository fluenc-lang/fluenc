#include "nodes/ExportedFunctionTerminatorNode.h"

std::vector<DzResult> ExportedFunctionTerminatorNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitExportedFunctionTerminator(this, entryPoint, values);
}

std::vector<DzResult> ExportedFunctionTerminatorNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitExportedFunctionTerminator(this, entryPoint, values);
}
