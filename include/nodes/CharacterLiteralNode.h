#ifndef CHARACTERLITERALNODE_H
#define CHARACTERLITERALNODE_H

#include <llvm/IR/Constants.h>

#include "Node.h"

class CharacterLiteralNode : public Node
{
	friend class Emitter;
	friend class Analyzer;

	public:
		CharacterLiteralNode(const Node *consumer, const std::string &value);

		std::vector<DzResult> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack values) const override;

	private:
		llvm::ConstantInt *getValue(llvm::Type *storageType) const;

		const Node *m_consumer;

		std::string m_value;
};

#endif // CHARACTERLITERALNODE_H
