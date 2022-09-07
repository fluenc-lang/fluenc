#ifndef EMITTER_H
#define EMITTER_H

#include "NodeVisitor.h"
#include "DzResult.h"

class BaseValue;

class Emitter : public DefaultNodeVisitor
{
	public:
		std::vector<DzResult> visit(const BooleanBinaryNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const FloatBinaryNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const IntegerBinaryNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const BinaryNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const ExportedFunctionNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const ArrayContinuationNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const ArrayElementNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const IntegralLiteralNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const FloatLiteralNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const BooleanLiteralNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const StringLiteralNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const CharacterLiteralNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const NothingNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const MemberAccessNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const ReferenceSinkNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const LazyEvaluationNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const FunctionCallNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const StackSegmentNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const FunctionCallProxyNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const JunctionNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const InstantiationNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const ConditionalNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const BlockInstructionNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const EmptyArrayNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const IndexSinkNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const ArraySinkNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const ExpansionNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const LocalNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const ContinuationNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const UnaryNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const TailFunctionCallNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const FunctionNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const ExportedFunctionTerminatorNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const ImportedFunctionNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const GlobalNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const ReturnNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const ParentInjectorNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const BlockStackFrameNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const TerminatorNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const IteratableNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const ArrayValue *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const IteratorValue *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visit(const StringIteratable *node, DefaultVisitorContext context) const override;
};

#endif // EMITTER_H
