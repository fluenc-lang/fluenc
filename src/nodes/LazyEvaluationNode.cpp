#include "IteratorStorage.h"
#include "nodes/LazyEvaluationNode.h"

#include "values/IteratorValue.h"
#include "values/ArrayValue.h"
#include "values/TupleValue.h"
#include "values/LazyValue.h"

LazyEvaluationNode::LazyEvaluationNode(const Node *consumer)
	: m_consumer(consumer)
{
}

LazyEvaluationNode::IDigestionNode *LazyEvaluationNode::processValue(IDigestionNode *node, const BaseValue *value) const
{
	if (auto groupNode = dynamic_cast<const GroupDigestionNode *>(node))
	{
		std::vector<IDigestionNode *> children;

		for (auto &child : groupNode->children())
		{
			auto processed = processValue(child, value);

			children.push_back(processed);
		}

		return new GroupDigestionNode(groupNode->type(), children);
	}

	if (auto valueNode = dynamic_cast<const ValueDigestionNode *>(node))
	{
		auto [resultEntryPoint, resultValues] = valueNode->result();

		std::vector<const BaseValue *> forwardedValues;

		for (auto resultValue : resultValues)
		{
			forwardedValues.push_back(resultValue);
		}

		forwardedValues.push_back(value);

		return new ValueDigestionNode({ resultEntryPoint, forwardedValues });
	}

	throw new std::exception();
}

LazyEvaluationNode::IDigestionNode *LazyEvaluationNode::processTuple(IDigestionNode *node, const Stack &values) const
{
	if (auto groupNode = dynamic_cast<const GroupDigestionNode *>(node))
	{
		std::vector<IDigestionNode *> children;

		for (auto &child : groupNode->children())
		{
			children.push_back(processTuple(child, values));
		}

		return new GroupDigestionNode(groupNode->type(), children);
	}

	if (auto valueNode = dynamic_cast<const ValueDigestionNode *>(node))
	{
		auto [digestedEntryPoint, digestedValues] = valueNode->result();

		auto digestedTuple = new TupleValue({ digestedValues.begin(), digestedValues.end() });
		auto digestedNode = digest(digestedEntryPoint, values);

		return processValue(digestedNode, digestedTuple);
	}

	throw new std::exception();
}

LazyEvaluationNode::IDigestionNode *LazyEvaluationNode::digest(const EntryPoint &entryPoint, Stack values) const
{
	for (auto i = 0u; i < values.size(); i++)
	{
		auto value = values.pop();

		if (auto lazy = dynamic_cast<const LazyValue *>(value))
		{
			auto iteratable = lazy->generate(entryPoint);

			std::vector<IDigestionNode *> results;

			for (auto &[resultEntryPoint, resultValues] : iteratable->build(entryPoint))
			{
				auto forwardedValues = values;

				for (auto &resultValue : resultValues)
				{
					forwardedValues.push(resultValue);
				}

				auto forwardedEntryPoint = resultEntryPoint
					.withIteratorStorage(entryPoint.iteratorStorage());

				auto digestedNode = digest(forwardedEntryPoint, forwardedValues);

				results.push_back(digestedNode);
			}

			return new GroupDigestionNode(Operator::And, results);
		}

		if (auto tuple = dynamic_cast<const TupleValue *>(value))
		{
			auto digestedNode = digest(entryPoint
				, tuple->values()
				);

			return processTuple(digestedNode, values);
		}

		auto digestedNode = digest(entryPoint, values);

		return processValue(digestedNode, value);
	}

	return new ValueDigestionNode({ entryPoint, values });
}

EntryPoint tryForkEntryPoint(const EntryPoint &entryPoint)
{
	if (entryPoint.iteratorStorage())
	{
		return entryPoint;
	}

	return entryPoint
		.withIteratorStorage(new IteratorStorage());
}

std::vector<DzResult> LazyEvaluationNode::execute(const std::vector<IDigestionNode *> &nodes, IteratorStorage *iteratorStorage, Operator type) const
{
	std::vector<DzResult> results;

	auto nodeIterator = [&](auto pair, auto node) -> std::pair<bool, std::exception *>
	{
		if (auto groupNode = dynamic_cast<const GroupDigestionNode *>(node))
		{
			for (auto &result : execute(groupNode->children(), iteratorStorage, groupNode->type()))
			{
				results.push_back(result);
			}
		}

		if (auto valueNode = dynamic_cast<const ValueDigestionNode *>(node))
		{
			auto [atLeastOneSucceeded, ex] = pair;

			try
			{
				auto [resultEntryPoint, resultValues] = valueNode->result();

				auto consumerEntryPoint = resultEntryPoint
					.withIteratorStorage(iteratorStorage);

				for (auto &result : m_consumer->build(consumerEntryPoint, resultValues))
				{
					results.push_back(result);
				}

				return { true, nullptr };
			}
			catch (std::exception *exception)
			{
				if (type == Operator::And)
				{
					throw exception;
				}

				return { atLeastOneSucceeded, exception };
			}
		}

		return pair;
	};

	auto input = std::make_pair<bool, std::exception *>(false, nullptr);

	auto [atLeastOneSucceeded, exception] = std::accumulate(begin(nodes), end(nodes), input, nodeIterator);

	if (type == Operator::And)
	{
		return results;
	}

	if (atLeastOneSucceeded)
	{
		return results;
	}

	throw exception;
}

std::vector<DzResult> LazyEvaluationNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto forkedEntryPoint = tryForkEntryPoint(entryPoint);

	auto digestedNode = digest(forkedEntryPoint, values);

	return execute({ digestedNode }, entryPoint.iteratorStorage(), Operator::And);
}
