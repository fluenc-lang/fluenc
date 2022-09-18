#ifndef FUNCTIONNODE_H
#define FUNCTIONNODE_H

#include "nodes/CallableNode.h"

class DzArgument;
class DzBaseArgument;
class BaseValue;

class FunctionNode : public CallableNode
{
	friend class Emitter;
	friend class Analyzer;
	friend class NodeLocator;

	struct Argument
	{
		std::string name;

		const BaseValue *value;
	};

	public:
		FunctionNode(FunctionAttribute attribute
			, const std::string &name
			, const std::vector<DzBaseArgument *> &arguments
			, Node *block
			);

		std::string name() const override;
		std::vector<DzBaseArgument *> arguments() const override;

		FunctionAttribute attribute() const override;

		int8_t signatureCompatibility(const EntryPoint &entryPoint, const std::vector<const Type *> &values) const override;

		std::vector<DzResult> accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const override;

		const Node *accept(const NodeLocator &visitor, EntryPoint context) const override;

	private:
		std::vector<Argument> handleArgument(DzBaseArgument *argument, const EntryPoint &entryPoint, const BaseValue *value) const;

		FunctionAttribute m_attribute;

		std::string m_name;
		std::vector<DzBaseArgument *> m_arguments;

		Node *m_block;
};


#endif // FUNCTIONNODE_H
