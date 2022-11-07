#ifndef TERMINATORNODE_H
#define TERMINATORNODE_H

#include "Node.h"
#include "FunctionAttribute.h"

class TerminatorNode : public VisitableNode<TerminatorNode>
{
	public:
		TerminatorNode(const std::string &name, FunctionAttribute attribute);

		FunctionAttribute attribute() const;

		static TerminatorNode *instance();

	private:
		std::string m_name;

		FunctionAttribute m_attribute;
};

#endif // TERMINATORNODE_H
