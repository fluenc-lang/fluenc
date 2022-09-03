#ifndef TERMINATORNODE_H
#define TERMINATORNODE_H

#include "Node.h"
#include "FunctionAttribute.h"

class TerminatorNode : public Node
{
	public:
		TerminatorNode(const std::string &name, FunctionAttribute attribute);

		std::vector<DzResult> accept(const Emitter &visitor, DefaultVisitorContext context) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, DefaultVisitorContext context) const override;

		FunctionAttribute attribute() const;

		static TerminatorNode *instance();

	private:
		std::string m_name;

		FunctionAttribute m_attribute;
};

#endif // TERMINATORNODE_H
