#include <numeric>

#include <llvm/IR/Instructions.h>

#include "nodes/ContinuationNode.h"
#include "Type.h"

#include "values/ExpandedValue.h"
#include "values/TupleValue.h"

template<typename Container>
std::vector<const ExpandedValue *> findExpandedValues(Container container)
{
	std::vector<const ExpandedValue *> values;

	for (auto i = container.rbegin(); i != container.rend(); i++)
	{
		if (auto tupleValue = dynamic_cast<const TupleValue *>(*i))
		{
			auto nested = findExpandedValues(tupleValue->values());

			for (auto &value : nested)
			{
				values.push_back(value);
			}
		}

		if (auto expandedValue = dynamic_cast<const ExpandedValue *>(*i))
		{
			values.push_back(expandedValue);
		}
	}

	return values;
}

ContinuationNode::ContinuationNode(const Node *node, const Type *iteratorType)
	: m_node(node)
	, m_iteratorType(iteratorType)
{
}
