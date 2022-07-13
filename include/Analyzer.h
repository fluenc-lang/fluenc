#ifndef ANALYZER_H
#define ANALYZER_H

#include "NodeVisitor.h"

class Analyzer : public NodeVisitor<BaseValue>
{
	public:
		Analyzer();

		std::vector<DzResult<BaseValue>> visitBinary(const BinaryNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitExportedFunction(const ExportedFunctionNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitArrayContinuation(const ArrayContinuationNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitArrayElement(const ArrayElementNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitIntegralLiteral(const IntegralLiteralNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitFloatLiteral(const FloatLiteralNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitBooleanLiteral(const BooleanLiteralNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitStringLiteral(const StringLiteralNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitCharacterLiteral(const CharacterLiteralNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitNothing(const NothingNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitMemberAccess(const MemberAccessNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitReferenceSink(const ReferenceSinkNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitLazyEvaluation(const LazyEvaluationNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitFunctionCall(const FunctionCallNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitStackSegment(const StackSegmentNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitFunctionCallProxy(const FunctionCallProxyNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitJunction(const JunctionNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitInstantiation(const InstantiationNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitConditional(const ConditionalNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitBlockInstruction(const BlockInstructionNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitEmptyArray(const EmptyArrayNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitIndexSink(const IndexSinkNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitArraySink(const ArraySinkNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitExpansion(const ExpansionNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitLocal(const LocalNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitContinuation(const ContinuationNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitUnary(const UnaryNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitTailFunctionCall(const TailFunctionCallNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitFunction(const FunctionNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitExportedFunctionTerminator(const ExportedFunctionTerminatorNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitImportedFunction(const ImportedFunctionNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitGlobal(const GlobalNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitReturn(const ReturnNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitParentInjector(const ParentInjectorNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitBlockStackFrame(const BlockStackFrameNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitTerminator(const TerminatorNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitIteratable(const IteratableNode *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitArrayValue(const ArrayValue *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitIteratorValue(const IteratorValue *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> visitStringIteratable(const StringIteratable *node, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
};

#endif // ANALYZER_H
