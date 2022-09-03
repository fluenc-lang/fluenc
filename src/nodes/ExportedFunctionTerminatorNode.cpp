#include "nodes/ExportedFunctionTerminatorNode.h"

std::vector<DzResult> ExportedFunctionTerminatorNode::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visitExportedFunctionTerminator(this, context);
}

std::vector<DzResult> ExportedFunctionTerminatorNode::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visitExportedFunctionTerminator(this, context);
}
