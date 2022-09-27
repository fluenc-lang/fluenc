#ifndef INTEGRALLITERALNODE_H
#define INTEGRALLITERALNODE_H

#include <llvm/IR/Constants.h>

#include "Node.h"

class ITypeName;

class IntegralLiteralNode : public VisitableNode<IntegralLiteralNode>
{
	friend class Emitter;

	public:
		IntegralLiteralNode(const Node *consumer, ITypeName *type, const std::string &value);

	private:
		llvm::ConstantInt *getValue(llvm::Type *storageType) const;

		const Node *m_consumer;
		const ITypeName *m_type;

		std::string m_value;
};

#endif // INTEGRALLITERALNODE_H
