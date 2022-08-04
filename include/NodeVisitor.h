#ifndef NODEVISITOR_H
#define NODEVISITOR_H

#include "Utility.h"
#include "DzResult.h"

class BinaryNode;
class ExportedFunctionNode;
class ArrayContinuationNode;
class ArrayElementNode;
class IntegralLiteralNode;
class FloatLiteralNode;
class BooleanLiteralNode;
class StringLiteralNode;
class CharacterLiteralNode;
class MemberAccessNode;
class ReferenceSinkNode;
class LazyEvaluationNode;
class FunctionCallNode;
class StackSegmentNode;
class FunctionCallProxyNode;
class JunctionNode;
class NothingNode;
class InstantiationNode;
class ConditionalNode;
class BlockInstructionNode;
class EmptyArrayNode;
class IndexSinkNode;
class ArraySinkNode;
class ExpansionNode;
class LocalNode;
class ContinuationNode;
class UnaryNode;
class TailFunctionCallNode;
class FunctionNode;
class ExportedFunctionTerminatorNode;
class ImportedFunctionNode;
class GlobalNode;
class ReturnNode;
class ParentInjectorNode;
class BlockStackFrameNode;
class TerminatorNode;
class IteratableNode;
class ArrayValue;
class IteratorValue;
class StringIteratable;
class EntryPoint;

template<typename TVisitor>
struct VisitableImpl
{
	virtual std::vector<DzResult> accept(const TVisitor &visitor, const EntryPoint &entryPoint, Stack values) const = 0;
};

template<typename ...TVisitor>
struct Visitable : public VisitableImpl<TVisitor>...
{
	using VisitableImpl<TVisitor>::accept...;
};

class NodeVisitor
{
	public:
		virtual std::vector<DzResult> visitBooleanBinary(const BinaryNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitFloatBinary(const BinaryNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitIntegerBinary(const BinaryNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitBinary(const BinaryNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitExportedFunction(const ExportedFunctionNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitArrayContinuation(const ArrayContinuationNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitArrayElement(const ArrayElementNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitIntegralLiteral(const IntegralLiteralNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitFloatLiteral(const FloatLiteralNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitBooleanLiteral(const BooleanLiteralNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitStringLiteral(const StringLiteralNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitCharacterLiteral(const CharacterLiteralNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitNothing(const NothingNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitMemberAccess(const MemberAccessNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitReferenceSink(const ReferenceSinkNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitLazyEvaluation(const LazyEvaluationNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitFunctionCall(const FunctionCallNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitStackSegment(const StackSegmentNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitFunctionCallProxy(const FunctionCallProxyNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitJunction(const JunctionNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitInstantiation(const InstantiationNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitConditional(const ConditionalNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitBlockInstruction(const BlockInstructionNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitEmptyArray(const EmptyArrayNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitIndexSink(const IndexSinkNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitArraySink(const ArraySinkNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitExpansion(const ExpansionNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitLocal(const LocalNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitContinuation(const ContinuationNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitUnary(const UnaryNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitTailFunctionCall(const TailFunctionCallNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitFunction(const FunctionNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitExportedFunctionTerminator(const ExportedFunctionTerminatorNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitImportedFunction(const ImportedFunctionNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitGlobal(const GlobalNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitReturn(const ReturnNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitParentInjector(const ParentInjectorNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitBlockStackFrame(const BlockStackFrameNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitTerminator(const TerminatorNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitIteratable(const IteratableNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitArrayValue(const ArrayValue *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitIteratorValue(const IteratorValue *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual std::vector<DzResult> visitStringIteratable(const StringIteratable *node, const EntryPoint &entryPoint, Stack values) const = 0;
};

#endif // NODEVISITOR_H
