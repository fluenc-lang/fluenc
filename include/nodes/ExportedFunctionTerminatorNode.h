#ifndef EXPORTEDFUNCTIONTERMINATORNODE_H
#define EXPORTEDFUNCTIONTERMINATORNODE_H

#include "Node.h"

class ExportedFunctionTerminatorNode : public Node
{
	public:
		std::vector<DzResult<BaseValue>> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
};

#endif // EXPORTEDFUNCTIONTERMINATORNODE_H
