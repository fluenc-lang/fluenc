#ifndef CHARACTERLITERALNODE_H
#define CHARACTERLITERALNODE_H

#include <llvm/IR/Constants.h>

#include "Node.h"

class CharacterLiteralNode : public Node
{
	public:
		CharacterLiteralNode(Node *consumer, const std::string &value);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		llvm::ConstantInt *getValue(llvm::Type *storageType) const;

		Node *m_consumer;

		std::string m_value;
};

#endif // CHARACTERLITERALNODE_H
