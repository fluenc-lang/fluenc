#ifndef EXPORTEDFUNCTIONTERMINATORNODE_H
#define EXPORTEDFUNCTIONTERMINATORNODE_H

#include "Node.h"

class ExportedFunctionTerminatorNode : public Node
{
	public:
		std::vector<DzResult> accept(const Emitter &visitor, DefaultVisitorContext context) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, DefaultVisitorContext context) const override;
};

#endif // EXPORTEDFUNCTIONTERMINATORNODE_H
