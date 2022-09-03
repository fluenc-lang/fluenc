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

		std::vector<DzResult> accept(const Emitter &visitor, DefaultVisitorContext context) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, DefaultVisitorContext context) const override;

	private:
		llvm::ConstantInt *getValue(llvm::Type *storageType) const;

		const Node *m_consumer;
		const ITypeName *m_type;

		std::string m_value;
};

#endif // INTEGRALLITERALNODE_H
