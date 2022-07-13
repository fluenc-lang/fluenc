#ifndef IMPORTEDFUNCTIONNODE_H
#define IMPORTEDFUNCTIONNODE_H

#include "nodes/CallableNode.h"

#include "TokenInfo.h"

class DzBaseArgument;
class ITypeName;

class ImportedFunctionNode : public CallableNode
{
	friend class Emitter;

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

		std::vector<DzResult<BaseValue>> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;

	private:
		ITypeName *m_returnType;

		std::string m_name;
		std::shared_ptr<peg::Ast> m_ast;
		std::vector<DzBaseArgument *> m_arguments;
};

#endif // IMPORTEDFUNCTIONNODE_H
