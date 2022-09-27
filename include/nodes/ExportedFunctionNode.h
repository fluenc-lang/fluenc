#ifndef EXPORTEDFUNCTIONNODE_H
#define EXPORTEDFUNCTIONNODE_H

#include "nodes/CallableNode.h"

class ITypeName;

class ExportedFunctionNode : public CallableNode
{
	friend class Emitter;

	public:
		ExportedFunctionNode(const std::string &name, Node *block, ITypeName *returnType);

		std::string name() const override;
		std::vector<DzBaseArgument *> arguments() const override;

		FunctionAttribute attribute() const override;

		int8_t signatureCompatibility(const EntryPoint &entryPoint, const std::vector<const Type *> &values) const override;

		std::vector<DzResult> accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const override;

	private:
		std::string m_name;

		Node *m_block;
		ITypeName *m_returnType;
};

#endif // EXPORTEDFUNCTIONNODE_H
