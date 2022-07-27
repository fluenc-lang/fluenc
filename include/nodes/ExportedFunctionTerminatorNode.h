#ifndef EXPORTEDFUNCTIONTERMINATORNODE_H
#define EXPORTEDFUNCTIONTERMINATORNODE_H

#include "Node.h"

class ExportedFunctionTerminatorNode : public Node
{
	public:
		std::vector<DzResult> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack values) const override;
};

#endif // EXPORTEDFUNCTIONTERMINATORNODE_H
