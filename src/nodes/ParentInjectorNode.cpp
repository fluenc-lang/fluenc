#include <numeric>

#include "Visitor.h"

#include "nodes/ParentInjectorNode.h"
#include "nodes/BlockStackFrameNode.h"
#include "nodes/BlockInstructionNode.h"

ParentInjectorNode::ParentInjectorNode(const std::vector<std::shared_ptr<peg::AstBase<peg::EmptyType> > > &nodes
	, const std::vector<std::string> &namespaces
	, const Type *iteratorType
	, const Node *alpha
	, const Node *beta
	)
	: m_subject(inject(nodes, namespaces, iteratorType, this, alpha, beta))
{
}

bool ParentInjectorNode::containsIterator() const
{
	return m_subject->containsIterator();
}

const IBlockInstruction *ParentInjectorNode::inject(const std::vector<std::shared_ptr<peg::AstBase<peg::EmptyType> > > &nodes, const std::vector<std::string> &namespaces, const Type *iteratorType, const Node *node, const Node *alpha, const Node *beta)
{
	Visitor visitor(namespaces, iteratorType, node, alpha, beta);

	auto first = rbegin(nodes);

	auto ret = visitor.visitReturn(*first);

	return std::accumulate(first + 1, rend(nodes), ret, [&](IBlockInstruction *consumer, auto expression) -> IBlockInstruction *
	{
		auto stackFrame = new BlockStackFrameNode(consumer);

		Visitor visitor(namespaces, iteratorType, node, stackFrame, alpha);

		auto value = visitor
			.visitExpression(expression);

		if (auto instruction = dynamic_cast<const IBlockInstruction *>(value))
		{
			return new BlockInstructionNode(instruction
				, instruction->containsIterator() || consumer->containsIterator()
				);
		}

		return new BlockInstructionNode(value
			, consumer->containsIterator()
			);
	});
}

std::vector<DzResult> ParentInjectorNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitParentInjector(this, entryPoint, values);
}
