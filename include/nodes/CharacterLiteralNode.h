#ifndef CHARACTERLITERALNODE_H
#define CHARACTERLITERALNODE_H

#include <llvm/IR/Constants.h>

#include "Node.h"

class CharacterLiteralNode : public VisitableNode<CharacterLiteralNode>
{
	friend class Emitter;
	friend class Analyzer;
	friend class NodeLocator;

	public:
		CharacterLiteralNode(const Node *consumer, const std::string &value);

	private:
		llvm::ConstantInt *getValue(llvm::Type *storageType) const;

		const Node *m_consumer;

		std::string m_value;
};

#endif // CHARACTERLITERALNODE_H
