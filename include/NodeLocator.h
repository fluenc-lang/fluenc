#ifndef NODELOCATOR_H
#define NODELOCATOR_H

#include "NodeVisitor.h"

class NodeLocator : public NodeVisitor<const Node *, EntryPoint>
{
	using Predicate = std::function<bool(const Node *)>;

	public:
		NodeLocator(Predicate predicate);

		const Node *visit(const BooleanBinaryNode *node, EntryPoint context) const override;
		const Node *visit(const FloatBinaryNode *node, EntryPoint context) const override;
		const Node *visit(const IntegerBinaryNode *node, EntryPoint context) const override;
		const Node *visit(const BinaryNode *node, EntryPoint context) const override;
		const Node *visit(const ExportedFunctionNode *node, EntryPoint context) const override;
		const Node *visit(const ArrayContinuationNode *node, EntryPoint context) const override;
		const Node *visit(const ArrayElementNode *node, EntryPoint context) const override;
		const Node *visit(const IntegralLiteralNode *node, EntryPoint context) const override;
		const Node *visit(const FloatLiteralNode *node, EntryPoint context) const override;
		const Node *visit(const BooleanLiteralNode *node, EntryPoint context) const override;
		const Node *visit(const StringLiteralNode *node, EntryPoint context) const override;
		const Node *visit(const CharacterLiteralNode *node, EntryPoint context) const override;
		const Node *visit(const NothingNode *node, EntryPoint context) const override;
		const Node *visit(const MemberAccessNode *node, EntryPoint context) const override;
		const Node *visit(const ReferenceSinkNode *node, EntryPoint context) const override;
		const Node *visit(const LazyEvaluationNode *node, EntryPoint context) const override;
		const Node *visit(const FunctionCallNode *node, EntryPoint context) const override;
		const Node *visit(const StackSegmentNode *node, EntryPoint context) const override;
		const Node *visit(const FunctionCallProxyNode *node, EntryPoint context) const override;
		const Node *visit(const JunctionNode *node, EntryPoint context) const override;
		const Node *visit(const InstantiationNode *node, EntryPoint context) const override;
		const Node *visit(const ConditionalNode *node, EntryPoint context) const override;
		const Node *visit(const BlockInstructionNode *node, EntryPoint context) const override;
		const Node *visit(const EmptyArrayNode *node, EntryPoint context) const override;
		const Node *visit(const IndexSinkNode *node, EntryPoint context) const override;
		const Node *visit(const ArraySinkNode *node, EntryPoint context) const override;
		const Node *visit(const ExpansionNode *node, EntryPoint context) const override;
		const Node *visit(const LocalNode *node, EntryPoint context) const override;
		const Node *visit(const ContinuationNode *node, EntryPoint context) const override;
		const Node *visit(const UnaryNode *node, EntryPoint context) const override;
		const Node *visit(const TailFunctionCallNode *node, EntryPoint context) const override;
		const Node *visit(const FunctionNode *node, EntryPoint context) const override;
		const Node *visit(const ExportedFunctionTerminatorNode *node, EntryPoint context) const override;
		const Node *visit(const ImportedFunctionNode *node, EntryPoint context) const override;
		const Node *visit(const GlobalNode *node, EntryPoint context) const override;
		const Node *visit(const ReturnNode *node, EntryPoint context) const override;
		const Node *visit(const ParentInjectorNode *node, EntryPoint context) const override;
		const Node *visit(const BlockStackFrameNode *node, EntryPoint context) const override;
		const Node *visit(const TerminatorNode *node, EntryPoint context) const override;
		const Node *visit(const IteratableNode *node, EntryPoint context) const override;
		const Node *visit(const ArrayValue *node, EntryPoint context) const override;
		const Node *visit(const IteratorValue *node, EntryPoint context) const override;
		const Node *visit(const StringIteratable *node, EntryPoint context) const override;

	private:
		Predicate m_predicate;
};

#endif // NODELOCATOR_H
