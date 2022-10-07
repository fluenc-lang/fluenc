#ifndef NODEVISITOR_H
#define NODEVISITOR_H

#include "DzResult.h"

struct BinaryNode;
struct IntegerBinaryNode;
struct FloatBinaryNode;
struct BooleanBinaryNode;
struct StringBinaryNode;
struct UnaryNode;
struct IntegerUnaryNode;
struct FloatUnaryNode;
struct BooleanUnaryNode;
struct StringUnaryNode;

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
class DistributorNode;

struct DefaultVisitorContext
{
	const EntryPoint &entryPoint;

	Stack values;
};

struct DummyVisitorContext
{
};

template<typename TReturn, typename TContext>
class NodeVisitor
{
	public:
		using ReturnType = TReturn;
		using ContextType = TContext;

		virtual TReturn visit(const BooleanBinaryNode *node, TContext context) const = 0;
		virtual TReturn visit(const FloatBinaryNode *node, TContext context) const = 0;
		virtual TReturn visit(const IntegerBinaryNode *node, TContext context) const = 0;
		virtual TReturn visit(const StringBinaryNode *node, TContext context) const = 0;
		virtual TReturn visit(const BinaryNode *node, TContext context) const = 0;
		virtual TReturn visit(const ExportedFunctionNode *node, TContext context) const = 0;
		virtual TReturn visit(const ArrayContinuationNode *node, TContext context) const = 0;
		virtual TReturn visit(const ArrayElementNode *node, TContext context) const = 0;
		virtual TReturn visit(const IntegralLiteralNode *node, TContext context) const = 0;
		virtual TReturn visit(const FloatLiteralNode *node, TContext context) const = 0;
		virtual TReturn visit(const BooleanLiteralNode *node, TContext context) const = 0;
		virtual TReturn visit(const StringLiteralNode *node, TContext context) const = 0;
		virtual TReturn visit(const CharacterLiteralNode *node, TContext context) const = 0;
		virtual TReturn visit(const NothingNode *node, TContext context) const = 0;
		virtual TReturn visit(const MemberAccessNode *node, TContext context) const = 0;
		virtual TReturn visit(const ReferenceSinkNode *node, TContext context) const = 0;
		virtual TReturn visit(const LazyEvaluationNode *node, TContext context) const = 0;
		virtual TReturn visit(const FunctionCallNode *node, TContext context) const = 0;
		virtual TReturn visit(const StackSegmentNode *node, TContext context) const = 0;
		virtual TReturn visit(const FunctionCallProxyNode *node, TContext context) const = 0;
		virtual TReturn visit(const JunctionNode *node, TContext context) const = 0;
		virtual TReturn visit(const InstantiationNode *node, TContext context) const = 0;
		virtual TReturn visit(const ConditionalNode *node, TContext context) const = 0;
		virtual TReturn visit(const BlockInstructionNode *node, TContext context) const = 0;
		virtual TReturn visit(const EmptyArrayNode *node, TContext context) const = 0;
		virtual TReturn visit(const IndexSinkNode *node, TContext context) const = 0;
		virtual TReturn visit(const ArraySinkNode *node, TContext context) const = 0;
		virtual TReturn visit(const ExpansionNode *node, TContext context) const = 0;
		virtual TReturn visit(const LocalNode *node, TContext context) const = 0;
		virtual TReturn visit(const ContinuationNode *node, TContext context) const = 0;
		virtual TReturn visit(const BooleanUnaryNode *node, TContext context) const = 0;
		virtual TReturn visit(const FloatUnaryNode *node, TContext context) const = 0;
		virtual TReturn visit(const IntegerUnaryNode *node, TContext context) const = 0;
		virtual TReturn visit(const StringUnaryNode *node, TContext context) const = 0;
		virtual TReturn visit(const UnaryNode *node, TContext context) const = 0;
		virtual TReturn visit(const TailFunctionCallNode *node, TContext context) const = 0;
		virtual TReturn visit(const FunctionNode *node, TContext context) const = 0;
		virtual TReturn visit(const ExportedFunctionTerminatorNode *node, TContext context) const = 0;
		virtual TReturn visit(const ImportedFunctionNode *node, TContext context) const = 0;
		virtual TReturn visit(const GlobalNode *node, TContext context) const = 0;
		virtual TReturn visit(const ReturnNode *node, TContext context) const = 0;
		virtual TReturn visit(const ParentInjectorNode *node, TContext context) const = 0;
		virtual TReturn visit(const BlockStackFrameNode *node, TContext context) const = 0;
		virtual TReturn visit(const TerminatorNode *node, TContext context) const = 0;
		virtual TReturn visit(const IteratableNode *node, TContext context) const = 0;
		virtual TReturn visit(const ArrayValue *node, TContext context) const = 0;
		virtual TReturn visit(const IteratorValue *node, TContext context) const = 0;
		virtual TReturn visit(const StringIteratable *node, TContext context) const = 0;
		virtual TReturn visit(const DistributorNode *node, TContext context) const = 0;
};

using DefaultNodeVisitor = NodeVisitor<std::vector<DzResult>, DefaultVisitorContext>;

#endif // NODEVISITOR_H
