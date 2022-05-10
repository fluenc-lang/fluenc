#ifndef TAILFUNCTIONCALLNODE_H
#define TAILFUNCTIONCALLNODE_H

#include <numeric>

#include "Node.h"

#include "values/UserTypeValue.h"
#include "values/ExpandedValue.h"
#include "values/TupleValue.h"
#include "values/NamedValue.h"

class TailFunctionCallNode : public Node
{
	public:
		TailFunctionCallNode(const std::vector<std::string> &names, const Node *regularCall);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

		int order(const EntryPoint &entryPoint) const override;

	private:
		std::vector<std::string> m_names;

		const Node *m_regularCall;
};

#endif // TAILFUNCTIONCALLNODE_H
