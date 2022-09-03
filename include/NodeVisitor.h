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

struct DefaultVisitorContext
{
	const EntryPoint &entryPoint;

	Stack values;
};

template<typename TVisitor>
struct VisitableImpl
{
	virtual std::vector<DzResult> accept(const TVisitor &visitor, DefaultVisitorContext context) const = 0;
};

template<typename ...TVisitor>
struct Visitable : public VisitableImpl<TVisitor>...
{
	using VisitableImpl<TVisitor>::accept...;
};

struct DummyVisitorContext
{
};

template<typename TReturn, typename TContext>
class NodeVisitor
{
	public:
		virtual TReturn visitBooleanBinary(const BinaryNode *node, TContext context) const = 0;
		virtual TReturn visitFloatBinary(const BinaryNode *node, TContext context) const = 0;
		virtual TReturn visitIntegerBinary(const BinaryNode *node, TContext context) const = 0;
		virtual TReturn visitBinary(const BinaryNode *node, TContext context) const = 0;
		virtual TReturn visitExportedFunction(const ExportedFunctionNode *node, TContext context) const = 0;
		virtual TReturn visitArrayContinuation(const ArrayContinuationNode *node, TContext context) const = 0;
		virtual TReturn visitArrayElement(const ArrayElementNode *node, TContext context) const = 0;
		virtual TReturn visitIntegralLiteral(const IntegralLiteralNode *node, TContext context) const = 0;
		virtual TReturn visitFloatLiteral(const FloatLiteralNode *node, TContext context) const = 0;
		virtual TReturn visitBooleanLiteral(const BooleanLiteralNode *node, TContext context) const = 0;
		virtual TReturn visitStringLiteral(const StringLiteralNode *node, TContext context) const = 0;
		virtual TReturn visitCharacterLiteral(const CharacterLiteralNode *node, TContext context) const = 0;
		virtual TReturn visitNothing(const NothingNode *node, TContext context) const = 0;
		virtual TReturn visitMemberAccess(const MemberAccessNode *node, TContext context) const = 0;
		virtual TReturn visitReferenceSink(const ReferenceSinkNode *node, TContext context) const = 0;
		virtual TReturn visitLazyEvaluation(const LazyEvaluationNode *node, TContext context) const = 0;
		virtual TReturn visitFunctionCall(const FunctionCallNode *node, TContext context) const = 0;
		virtual TReturn visitStackSegment(const StackSegmentNode *node, TContext context) const = 0;
		virtual TReturn visitFunctionCallProxy(const FunctionCallProxyNode *node, TContext context) const = 0;
		virtual TReturn visitJunction(const JunctionNode *node, TContext context) const = 0;
		virtual TReturn visitInstantiation(const InstantiationNode *node, TContext context) const = 0;
		virtual TReturn visitConditional(const ConditionalNode *node, TContext context) const = 0;
		virtual TReturn visitBlockInstruction(const BlockInstructionNode *node, TContext context) const = 0;
		virtual TReturn visitEmptyArray(const EmptyArrayNode *node, TContext context) const = 0;
		virtual TReturn visitIndexSink(const IndexSinkNode *node, TContext context) const = 0;
		virtual TReturn visitArraySink(const ArraySinkNode *node, TContext context) const = 0;
		virtual TReturn visitExpansion(const ExpansionNode *node, TContext context) const = 0;
		virtual TReturn visitLocal(const LocalNode *node, TContext context) const = 0;
		virtual TReturn visitContinuation(const ContinuationNode *node, TContext context) const = 0;
		virtual TReturn visitUnary(const UnaryNode *node, TContext context) const = 0;
		virtual TReturn visitTailFunctionCall(const TailFunctionCallNode *node, TContext context) const = 0;
		virtual TReturn visitFunction(const FunctionNode *node, TContext context) const = 0;
		virtual TReturn visitExportedFunctionTerminator(const ExportedFunctionTerminatorNode *node, TContext context) const = 0;
		virtual TReturn visitImportedFunction(const ImportedFunctionNode *node, TContext context) const = 0;
		virtual TReturn visitGlobal(const GlobalNode *node, TContext context) const = 0;
		virtual TReturn visitReturn(const ReturnNode *node, TContext context) const = 0;
		virtual TReturn visitParentInjector(const ParentInjectorNode *node, TContext context) const = 0;
		virtual TReturn visitBlockStackFrame(const BlockStackFrameNode *node, TContext context) const = 0;
		virtual TReturn visitTerminator(const TerminatorNode *node, TContext context) const = 0;
		virtual TReturn visitIteratable(const IteratableNode *node, TContext context) const = 0;
		virtual TReturn visitArrayValue(const ArrayValue *node, TContext context) const = 0;
		virtual TReturn visitIteratorValue(const IteratorValue *node, TContext context) const = 0;
		virtual TReturn visitStringIteratable(const StringIteratable *node, TContext context) const = 0;
};

using DefaultNodeVisitor = NodeVisitor<std::vector<DzResult>, DefaultVisitorContext>;

#endif // NODEVISITOR_H
