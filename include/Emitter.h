#ifndef EMITTER_H
#define EMITTER_H

#include "NodeVisitor.h"
#include "DzResult.h"

class BaseValue;

class Emitter : public NodeVisitor<BaseValue>
{
	public:
		std::vector<DzResult> visitBinary(const BinaryNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitExportedFunction(const ExportedFunctionNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitArrayContinuation(const ArrayContinuationNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitArrayElement(const ArrayElementNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitIntegralLiteral(const IntegralLiteralNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitFloatLiteral(const FloatLiteralNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitBooleanLiteral(const BooleanLiteralNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitStringLiteral(const StringLiteralNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitCharacterLiteral(const CharacterLiteralNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitNothing(const NothingNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitMemberAccess(const MemberAccessNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitReferenceSink(const ReferenceSinkNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitLazyEvaluation(const LazyEvaluationNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitFunctionCall(const FunctionCallNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitStackSegment(const StackSegmentNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitFunctionCallProxy(const FunctionCallProxyNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitJunction(const JunctionNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitInstantiation(const InstantiationNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitConditional(const ConditionalNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitBlockInstruction(const BlockInstructionNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitEmptyArray(const EmptyArrayNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitIndexSink(const IndexSinkNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitArraySink(const ArraySinkNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitExpansion(const ExpansionNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitLocal(const LocalNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitContinuation(const ContinuationNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitUnary(const UnaryNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitTailFunctionCall(const TailFunctionCallNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitFunction(const FunctionNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitExportedFunctionTerminator(const ExportedFunctionTerminatorNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitImportedFunction(const ImportedFunctionNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitGlobal(const GlobalNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitReturn(const ReturnNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitParentInjector(const ParentInjectorNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitBlockStackFrame(const BlockStackFrameNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitTerminator(const TerminatorNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitIteratable(const IteratableNode *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitArrayValue(const ArrayValue *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitIteratorValue(const IteratorValue *node, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> visitStringIteratable(const StringIteratable *node, const EntryPoint &entryPoint, Stack values) const override;
};

#endif // EMITTER_H
