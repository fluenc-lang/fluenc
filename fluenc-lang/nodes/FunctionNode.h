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
		FunctionNode(const std::string &name
			, const std::vector<DzBaseArgument *> &arguments
			, const std::vector<std::shared_ptr<peg::AstBase<peg::EmptyType> > > &nodes
			, const std::vector<std::string> &namespaces
			, const Type *iteratorType
			);

		ITypeName *returnType() const override;

		std::string name() const override;
		std::vector<DzBaseArgument *> arguments() const override;

		FunctionAttribute attribute() const override;

		int8_t signatureCompatibility(const EntryPoint &entryPoint, const std::vector<const Type *> &values) const override;

		std::vector<DzResult> accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const override;

	private:
		static IBlockInstruction *inject(const std::vector<std::shared_ptr<peg::AstBase<peg::EmptyType>>> &nodes
			, const std::vector<std::string> &namespaces
			, const Type *iteratorType
			, const Node *self
			);

		std::vector<Argument> handleArgument(DzBaseArgument *argument, const EntryPoint &entryPoint, const BaseValue *value) const;

		std::string m_name;
		std::vector<DzBaseArgument *> m_arguments;

		IBlockInstruction *m_block;
};


#endif // FUNCTIONNODE_H
