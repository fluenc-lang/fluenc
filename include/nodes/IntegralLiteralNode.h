#ifndef INTEGRALLITERALNODE_H
#define INTEGRALLITERALNODE_H

#include <llvm/IR/Constants.h>

#include "Node.h"

class ITypeName;

class IntegralLiteralNode : public Node
{
	friend class Emitter;
	friend class Analyzer;

	public:
		IntegralLiteralNode(const Node *consumer, ITypeName *type, const std::string &value);

		std::vector<DzResult<BaseValue>> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;

	private:
		llvm::ConstantInt *getValue(llvm::Type *storageType) const;

		const Node *m_consumer;
		const ITypeName *m_type;

		std::string m_value;
};

#endif // INTEGRALLITERALNODE_H
