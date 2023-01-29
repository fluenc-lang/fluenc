#ifndef EXPORTEDFUNCTIONTERMINATORNODE_H
#define EXPORTEDFUNCTIONTERMINATORNODE_H

#include "Node.h"

class ExportedFunctionTerminatorNode : public VisitableNode<ExportedFunctionTerminatorNode>
{
	friend class Emitter;

	public:
		ExportedFunctionTerminatorNode(const std::shared_ptr<peg::Ast> &ast);

	private:
		const std::shared_ptr<peg::Ast> m_ast;
};

#endif // EXPORTEDFUNCTIONTERMINATORNODE_H
