#ifndef TERMINATORNODE_H
#define TERMINATORNODE_H

#include "Node.h"
#include "FunctionAttribute.h"

class TerminatorNode : public Node
{
	public:
		TerminatorNode(const std::string &name, FunctionAttribute attribute);

		std::vector<DzResult<BaseValue>> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;

		FunctionAttribute attribute() const;

		static TerminatorNode *instance();

	private:
		std::string m_name;

		FunctionAttribute m_attribute;
};

#endif // TERMINATORNODE_H
