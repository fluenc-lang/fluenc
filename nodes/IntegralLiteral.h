#ifndef INTEGRALLITERAL_H
#define INTEGRALLITERAL_H

#include <llvm/IR/Constants.h>

#include "Node.h"

class ITypeName;

class IntegralLiteral : public Node
{
	public:
		IntegralLiteral(Node *consumer, ITypeName *type, const std::string &value);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		llvm::ConstantInt *getValue(llvm::Type *storageType) const;

		Node *m_consumer;
		ITypeName *m_type;

		std::string m_value;
};

#endif // INTEGRALLITERAL_H
