#ifndef LAZYEVALUATIONNODE_H
#define LAZYEVALUATIONNODE_H

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

	class IDigestionNode
	{
		public:
			virtual ~IDigestionNode() = default;
	};

	class GroupDigestionNode : public IDigestionNode
	{
		public:
			GroupDigestionNode(Operator type, const std::vector<IDigestionNode *> &children)
				: m_type(type)
				, m_children(children)
			{
			}

			Operator type() const
			{
				return m_type;
			}

			std::vector<IDigestionNode *> children() const
			{
				return m_children;
			}

		private:
			Operator m_type;

			std::vector<IDigestionNode *> m_children;
	};

	class ValueDigestionNode : public IDigestionNode
	{
		public:
			ValueDigestionNode(const DzResult &result)
				: m_result(result)
			{
			}

			DzResult result() const
			{
				return m_result;
			}

		private:
			DzResult m_result;
	};

	public:
		LazyEvaluationNode(const Node *consumer);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		IDigestionNode *processValue(IDigestionNode *node, const BaseValue *value) const;
		IDigestionNode *processTuple(IDigestionNode *node, const Stack &values) const;

		IDigestionNode *digest(const EntryPoint &entryPoint, Stack values) const;

		std::vector<DzResult> execute(const std::vector<IDigestionNode *> &nodes, IteratorStorage *iteratorStorage, Operator type) const;

		const Node *m_consumer;
};

#endif // LAZYEVALUATIONNODE_H
