#ifndef NODEVISITOR_H
#define NODEVISITOR_H

#include "Utility.h"
#include "Stack.h"

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
class VisitableImpl
{
	public:
		virtual typename TVisitor::ReturnType accept(const TVisitor &visitor, const EntryPoint &entryPoint, Stack values) const = 0;
};

template<typename ...TVisitor>
class Visitable : public VisitableImpl<TVisitor>...
{
};

template<typename T>
class NodeVisitor
{
	public:
		using ReturnType = T;

		virtual T visitBinary(const BinaryNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitExportedFunction(const ExportedFunctionNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitArrayContinuation(const ArrayContinuationNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitArrayElement(const ArrayElementNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitIntegralLiteral(const IntegralLiteralNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitFloatLiteral(const FloatLiteralNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitBooleanLiteral(const BooleanLiteralNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitStringLiteral(const StringLiteralNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitCharacterLiteral(const CharacterLiteralNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitNothing(const NothingNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitMemberAccess(const MemberAccessNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitReferenceSink(const ReferenceSinkNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitLazyEvaluation(const LazyEvaluationNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitFunctionCall(const FunctionCallNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitStackSegment(const StackSegmentNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitFunctionCallProxy(const FunctionCallProxyNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitJunction(const JunctionNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitInstantiation(const InstantiationNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitConditional(const ConditionalNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitBlockInstruction(const BlockInstructionNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitEmptyArray(const EmptyArrayNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitIndexSink(const IndexSinkNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitArraySink(const ArraySinkNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitExpansion(const ExpansionNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitLocal(const LocalNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitContinuation(const ContinuationNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitUnary(const UnaryNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitTailFunctionCall(const TailFunctionCallNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitFunction(const FunctionNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitExportedFunctionTerminator(const ExportedFunctionTerminatorNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitImportedFunction(const ImportedFunctionNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitGlobal(const GlobalNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitReturn(const ReturnNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitParentInjector(const ParentInjectorNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitBlockStackFrame(const BlockStackFrameNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitTerminator(const TerminatorNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitIteratable(const IteratableNode *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitArrayValue(const ArrayValue *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitIteratorValue(const IteratorValue *node, const EntryPoint &entryPoint, Stack values) const = 0;
		virtual T visitStringIteratable(const StringIteratable *node, const EntryPoint &entryPoint, Stack values) const = 0;
};

#endif // NODEVISITOR_H
