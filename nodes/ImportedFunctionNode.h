#ifndef IMPORTEDFUNCTIONNODE_H
#define IMPORTEDFUNCTIONNODE_H

#include "nodes/CallableNode.h"

class DzBaseArgument;
class ITypeName;

class ImportedFunctionNode : public CallableNode
{
	public:
		ImportedFunctionNode(antlr4::ParserRuleContext *context
			, const std::string &name
			, const std::vector<DzBaseArgument *> &arguments
			, ITypeName *returnType
			);

		std::string name() const override;
		std::vector<DzBaseArgument *> arguments() const override;

		FunctionAttribute attribute() const override;

		int8_t signatureCompatibility(const EntryPoint &entryPoint, const Stack &values) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		antlr4::ParserRuleContext *m_context;

		std::string m_name;
		std::vector<DzBaseArgument *> m_arguments;

		ITypeName *m_returnType;
};

#endif // IMPORTEDFUNCTIONNODE_H
