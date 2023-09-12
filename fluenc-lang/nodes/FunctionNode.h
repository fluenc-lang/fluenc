#ifndef FUNCTIONNODE_H
#define FUNCTIONNODE_H

#include "CallableNode.h"

class DzArgument;
class DzBaseArgument;
class BaseValue;
class IBlockInstruction;

class FunctionNode : public CallableNode
{
	friend class Emitter;

	struct Argument
	{
		std::string name;

		const BaseValue *value;
	};

	public:
		FunctionNode(const std::shared_ptr<peg::Ast> ast
			, const std::string &name
			, const std::vector<DzBaseArgument *> &arguments
			, const IBlockInstruction *block
			);

		const std::shared_ptr<peg::Ast> ast() const override;

		ITypeName *returnType() const override;

		std::string name() const override;
		std::vector<DzBaseArgument *> arguments() const override;

		FunctionAttribute attribute() const override;

		int8_t signatureCompatibility(const EntryPoint &entryPoint, const std::vector<const Type *> &values) const override;

		std::vector<DzResult> accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const override;

	private:
		std::vector<Argument> handleArgument(DzBaseArgument *argument, const EntryPoint &entryPoint, const BaseValue *value) const;

		const std::shared_ptr<peg::Ast> m_ast;

		std::string m_name;
		std::vector<DzBaseArgument *> m_arguments;

		const IBlockInstruction *m_block;
};


#endif // FUNCTIONNODE_H
