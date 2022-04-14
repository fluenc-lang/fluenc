#ifndef LAZYEVALUATIONNODE_H
#define LAZYEVALUATIONNODE_H

#include <variant>

#include "Node.h"

class IIteratable;
class LazyValue;

class LazyEvaluationNode : public Node
{
	enum class Operator
	{
		And,
		Or,
	};

	struct Value;
	struct Group;

	using DigestionNode = std::variant<Group, Value>;

	struct Value
	{
		DzResult result;
	};

	struct Group
	{
		Operator type;

		std::vector<DigestionNode> children;
	};

	public:
		LazyEvaluationNode(const Node *consumer);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		DigestionNode processValue(const DigestionNode &node, const BaseValue *value) const;
		DigestionNode processTuple(const DigestionNode &node, const Stack &values) const;

		DigestionNode digest(const EntryPoint &entryPoint, Stack values) const;

		std::vector<DzResult> execute(const std::vector<DigestionNode> &nodes, IteratorStorage *iteratorStorage, Operator type) const;

		const Node *m_consumer;
};

#endif // LAZYEVALUATIONNODE_H
