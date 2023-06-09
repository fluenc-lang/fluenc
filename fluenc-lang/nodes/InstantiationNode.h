#ifndef INSTANTIATIONNODE_H
#define INSTANTIATIONNODE_H

#include <unordered_set>

#include <llvm/IR/Instructions.h>

#include "Node.h"
#include "DzTypeName.h"

class DzAssignment;
class IPrototypeProvider;

class InstantiationNode : public VisitableNode<InstantiationNode>
{
	friend class Emitter;

	public:
		InstantiationNode(const Node *consumer
			, const IPrototypeProvider *prototypeProvider
			, const std::shared_ptr<peg::Ast> &ast
			, const std::vector<std::string> &fields
			);

	private:
		const Node *m_consumer;
		const IPrototypeProvider *m_prototypeProvider;

		std::shared_ptr<peg::Ast> m_ast;
		std::vector<std::string> m_fields;
};

#endif // INSTANTIATIONNODE_H
