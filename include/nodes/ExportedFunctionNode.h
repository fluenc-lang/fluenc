#ifndef EXPORTEDFUNCTIONNODE_H
#define EXPORTEDFUNCTIONNODE_H

#include "nodes/CallableNode.h"

class ITypeName;

class ExportedFunctionNode : public CallableNode
{
	public:
		ExportedFunctionNode(const std::string &name, Node *block, ITypeName *returnType);

		std::string name() const override;
		std::vector<DzBaseArgument *> arguments() const override;

		FunctionAttribute attribute() const override;

		int8_t signatureCompatibility(const EntryPoint &entryPoint, const Stack &values) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		std::string m_name;

		Node *m_block;
		ITypeName *m_returnType;
};

#endif // EXPORTEDFUNCTIONNODE_H
