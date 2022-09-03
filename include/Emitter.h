#ifndef EMITTER_H
#define EMITTER_H

#include "NodeVisitor.h"
#include "DzResult.h"

class BaseValue;

class Emitter : public DefaultNodeVisitor
{
	public:
		std::vector<DzResult> visitBooleanBinary(const BinaryNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitFloatBinary(const BinaryNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitIntegerBinary(const BinaryNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitBinary(const BinaryNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitExportedFunction(const ExportedFunctionNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitArrayContinuation(const ArrayContinuationNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitArrayElement(const ArrayElementNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitIntegralLiteral(const IntegralLiteralNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitFloatLiteral(const FloatLiteralNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitBooleanLiteral(const BooleanLiteralNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitStringLiteral(const StringLiteralNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitCharacterLiteral(const CharacterLiteralNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitNothing(const NothingNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitMemberAccess(const MemberAccessNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitReferenceSink(const ReferenceSinkNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitLazyEvaluation(const LazyEvaluationNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitFunctionCall(const FunctionCallNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitStackSegment(const StackSegmentNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitFunctionCallProxy(const FunctionCallProxyNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitJunction(const JunctionNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitInstantiation(const InstantiationNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitConditional(const ConditionalNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitBlockInstruction(const BlockInstructionNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitEmptyArray(const EmptyArrayNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitIndexSink(const IndexSinkNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitArraySink(const ArraySinkNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitExpansion(const ExpansionNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitLocal(const LocalNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitContinuation(const ContinuationNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitUnary(const UnaryNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitTailFunctionCall(const TailFunctionCallNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitFunction(const FunctionNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitExportedFunctionTerminator(const ExportedFunctionTerminatorNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitImportedFunction(const ImportedFunctionNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitGlobal(const GlobalNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitReturn(const ReturnNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitParentInjector(const ParentInjectorNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitBlockStackFrame(const BlockStackFrameNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitTerminator(const TerminatorNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitIteratable(const IteratableNode *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitArrayValue(const ArrayValue *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitIteratorValue(const IteratorValue *node, DefaultVisitorContext context) const override;
		std::vector<DzResult> visitStringIteratable(const StringIteratable *node, DefaultVisitorContext context) const override;
};

#endif // EMITTER_H
