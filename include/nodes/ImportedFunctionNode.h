#ifndef IMPORTEDFUNCTIONNODE_H
#define IMPORTEDFUNCTIONNODE_H

#include "nodes/CallableNode.h"

#include "TokenInfo.h"

class DzBaseArgument;
class ITypeName;

class ImportedFunctionNode : public CallableNode
{
	friend class Emitter;
	friend class Analyzer;

	public:
		ImportedFunctionNode(ITypeName *returnType
			, const std::string &name
			, const std::shared_ptr<peg::Ast> &ast
			, const std::vector<DzBaseArgument *> &arguments
			);

		std::string name() const override;
		std::vector<DzBaseArgument *> arguments() const override;

		FunctionAttribute attribute() const override;

		int8_t signatureCompatibility(const EntryPoint &entryPoint, const std::vector<const Type *> &values) const override;

		std::vector<DzResult> accept(const Emitter &visitor, DefaultVisitorContext context) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, DefaultVisitorContext context) const override;

		const Node *accept(const NodeLocator &visitor, EntryPoint context) const override;

	private:
		ITypeName *m_returnType;

		std::string m_name;
		std::shared_ptr<peg::Ast> m_ast;
		std::vector<DzBaseArgument *> m_arguments;
};

#endif // IMPORTEDFUNCTIONNODE_H
