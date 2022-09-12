#ifndef NODELOCATOR_H
#define NODELOCATOR_H

#include "NodeVisitor.h"

class NodeLocator : public NodeVisitor<const Node *, DummyVisitorContext>
{
	using Predicate = std::function<bool(const Node *)>;

	public:
		NodeLocator(Predicate predicate);

		const Node *visit(const BooleanBinaryNode *node, DummyVisitorContext context) const override;
		const Node *visit(const FloatBinaryNode *node, DummyVisitorContext context) const override;
		const Node *visit(const IntegerBinaryNode *node, DummyVisitorContext context) const override;
		const Node *visit(const BinaryNode *node, DummyVisitorContext context) const override;
		const Node *visit(const ExportedFunctionNode *node, DummyVisitorContext context) const override;
		const Node *visit(const ArrayContinuationNode *node, DummyVisitorContext context) const override;
		const Node *visit(const ArrayElementNode *node, DummyVisitorContext context) const override;
		const Node *visit(const IntegralLiteralNode *node, DummyVisitorContext context) const override;
		const Node *visit(const FloatLiteralNode *node, DummyVisitorContext context) const override;
		const Node *visit(const BooleanLiteralNode *node, DummyVisitorContext context) const override;
		const Node *visit(const StringLiteralNode *node, DummyVisitorContext context) const override;
		const Node *visit(const CharacterLiteralNode *node, DummyVisitorContext context) const override;
		const Node *visit(const NothingNode *node, DummyVisitorContext context) const override;
		const Node *visit(const MemberAccessNode *node, DummyVisitorContext context) const override;
		const Node *visit(const ReferenceSinkNode *node, DummyVisitorContext context) const override;
		const Node *visit(const LazyEvaluationNode *node, DummyVisitorContext context) const override;
		const Node *visit(const FunctionCallNode *node, DummyVisitorContext context) const override;
		const Node *visit(const StackSegmentNode *node, DummyVisitorContext context) const override;
		const Node *visit(const FunctionCallProxyNode *node, DummyVisitorContext context) const override;
		const Node *visit(const JunctionNode *node, DummyVisitorContext context) const override;
		const Node *visit(const InstantiationNode *node, DummyVisitorContext context) const override;
		const Node *visit(const ConditionalNode *node, DummyVisitorContext context) const override;
		const Node *visit(const BlockInstructionNode *node, DummyVisitorContext context) const override;
		const Node *visit(const EmptyArrayNode *node, DummyVisitorContext context) const override;
		const Node *visit(const IndexSinkNode *node, DummyVisitorContext context) const override;
		const Node *visit(const ArraySinkNode *node, DummyVisitorContext context) const override;
		const Node *visit(const ExpansionNode *node, DummyVisitorContext context) const override;
		const Node *visit(const LocalNode *node, DummyVisitorContext context) const override;
		const Node *visit(const ContinuationNode *node, DummyVisitorContext context) const override;
		const Node *visit(const UnaryNode *node, DummyVisitorContext context) const override;
		const Node *visit(const TailFunctionCallNode *node, DummyVisitorContext context) const override;
		const Node *visit(const FunctionNode *node, DummyVisitorContext context) const override;
		const Node *visit(const ExportedFunctionTerminatorNode *node, DummyVisitorContext context) const override;
		const Node *visit(const ImportedFunctionNode *node, DummyVisitorContext context) const override;
		const Node *visit(const GlobalNode *node, DummyVisitorContext context) const override;
		const Node *visit(const ReturnNode *node, DummyVisitorContext context) const override;
		const Node *visit(const ParentInjectorNode *node, DummyVisitorContext context) const override;
		const Node *visit(const BlockStackFrameNode *node, DummyVisitorContext context) const override;
		const Node *visit(const TerminatorNode *node, DummyVisitorContext context) const override;
		const Node *visit(const IteratableNode *node, DummyVisitorContext context) const override;
		const Node *visit(const ArrayValue *node, DummyVisitorContext context) const override;
		const Node *visit(const IteratorValue *node, DummyVisitorContext context) const override;
		const Node *visit(const StringIteratable *node, DummyVisitorContext context) const override;

	private:
		Predicate m_predicate;
};

#endif // NODELOCATOR_H
