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
	virtual std::vector<DzResult<typename TVisitor::ReturnType>> accept(const TVisitor &visitor, const EntryPoint &entryPoint, Stack<typename TVisitor::ReturnType> values) const = 0;
};

template<typename ...TVisitor>
struct Visitable : public VisitableImpl<TVisitor>...
{
	using VisitableImpl<TVisitor>::accept...;
};

template<typename T>
class NodeVisitor
{
	public:
		using ReturnType = T;

		virtual std::vector<DzResult<T>> visitBinary(const BinaryNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitExportedFunction(const ExportedFunctionNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitArrayContinuation(const ArrayContinuationNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitArrayElement(const ArrayElementNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitIntegralLiteral(const IntegralLiteralNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitFloatLiteral(const FloatLiteralNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitBooleanLiteral(const BooleanLiteralNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitStringLiteral(const StringLiteralNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitCharacterLiteral(const CharacterLiteralNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitNothing(const NothingNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitMemberAccess(const MemberAccessNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitReferenceSink(const ReferenceSinkNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitLazyEvaluation(const LazyEvaluationNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitFunctionCall(const FunctionCallNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitStackSegment(const StackSegmentNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitFunctionCallProxy(const FunctionCallProxyNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitJunction(const JunctionNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitInstantiation(const InstantiationNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitConditional(const ConditionalNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitBlockInstruction(const BlockInstructionNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitEmptyArray(const EmptyArrayNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitIndexSink(const IndexSinkNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitArraySink(const ArraySinkNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitExpansion(const ExpansionNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitLocal(const LocalNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitContinuation(const ContinuationNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitUnary(const UnaryNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitTailFunctionCall(const TailFunctionCallNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitFunction(const FunctionNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitExportedFunctionTerminator(const ExportedFunctionTerminatorNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitImportedFunction(const ImportedFunctionNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitGlobal(const GlobalNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitReturn(const ReturnNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitParentInjector(const ParentInjectorNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitBlockStackFrame(const BlockStackFrameNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitTerminator(const TerminatorNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitIteratable(const IteratableNode *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitArrayValue(const ArrayValue *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitIteratorValue(const IteratorValue *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
		virtual std::vector<DzResult<T>> visitStringIteratable(const StringIteratable *node, const EntryPoint &entryPoint, Stack<T> values) const = 0;
};

#endif // NODEVISITOR_H
