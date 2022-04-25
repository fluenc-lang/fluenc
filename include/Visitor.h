#ifndef VISITORV2_H
#define VISITORV2_H

#include <memory>
#include <any>
#include <vector>

#include "types/PrototypeFieldEmbryo.h"

namespace peg
{
	template <typename Annotation> struct AstBase;

	struct EmptyType;
	using Ast = AstBase<EmptyType>;
}

struct ModuleInfo;

class Node;
class CallableNode;
class Prototype;
class GlobalNode;
class Namespace;
class Use;
class DzBaseArgument;
class Type;
class BlockInstructionNode;
class ContinuationNode;
class ITypeName;

class Visitor
{
	public:
		Visitor(const std::vector<std::string> &namespaces
			, const Type *iteratorType
			, const Node *alpha
			, const Node *beta
			);

		ModuleInfo visit(const std::shared_ptr<peg::Ast> &ast) const;

	private:
		std::any visitInstruction(const std::shared_ptr<peg::Ast> &ast) const;

		std::string visitId(const std::shared_ptr<peg::Ast> &ast) const;
		std::string visitInteger(const std::shared_ptr<peg::Ast> &ast) const;
		std::string visitString(const std::shared_ptr<peg::Ast> &ast) const;

		std::vector<ITypeName *> visitTypeList(const std::shared_ptr<peg::Ast> &ast) const;
		std::vector<PrototypeFieldEmbryo> visitFieldList(const std::shared_ptr<peg::Ast> &ast) const;
		std::vector<std::string> visitIdList(const std::shared_ptr<peg::Ast> &ast) const;

		Node *visitExpression(const std::shared_ptr<peg::Ast> &ast) const;
		Node *visitLiteral(const std::shared_ptr<peg::Ast> &ast) const;
		Node *visitInt32Literal(const std::shared_ptr<peg::Ast> &ast) const;
		Node *visitInt64Literal(const std::shared_ptr<peg::Ast> &ast) const;
		Node *visitBooleanLiteral(const std::shared_ptr<peg::Ast> &ast) const;
		Node *visitStringLiteral(const std::shared_ptr<peg::Ast> &ast) const;
		Node *visitUint32Literal(const std::shared_ptr<peg::Ast> &ast) const;
		Node *visitCharLiteral(const std::shared_ptr<peg::Ast> &ast) const;
		Node *visitByteLiteral(const std::shared_ptr<peg::Ast> &ast) const;
		Node *visitNothingLiteral(const std::shared_ptr<peg::Ast> &ast) const;
		Node *visitBinary(const std::shared_ptr<peg::Ast> &ast) const;
		Node *visitMember(const std::shared_ptr<peg::Ast> &ast) const;
		Node *visitCall(const std::shared_ptr<peg::Ast> &ast) const;
		Node *visitInstantiation(const std::shared_ptr<peg::Ast> &ast) const;
		Node *visitConditional(const std::shared_ptr<peg::Ast> &ast) const;
		Node *visitArray(const std::shared_ptr<peg::Ast> &ast) const;
		Node *visitGroup(const std::shared_ptr<peg::Ast> &ast) const;
		Node *visitExpansion(const std::shared_ptr<peg::Ast> &ast) const;
		Node *visitLocal(const std::shared_ptr<peg::Ast> &ast) const;
		Node *visitContinuation(const std::shared_ptr<peg::Ast> &ast) const;
		Node *visitWith(const std::shared_ptr<peg::Ast> &ast) const;

		CallableNode *visitFunction(const std::shared_ptr<peg::Ast> &ast) const;
		CallableNode *visitRegularFunction(const std::shared_ptr<peg::Ast> &ast) const;
		CallableNode *visitExportedFunction(const std::shared_ptr<peg::Ast> &ast) const;
		CallableNode *visitImportedFunction(const std::shared_ptr<peg::Ast> &ast) const;

		DzBaseArgument *visitArgument(const std::shared_ptr<peg::Ast> &ast) const;
		DzBaseArgument *visitStandardArgument(const std::shared_ptr<peg::Ast> &ast) const;
		DzBaseArgument *visitTupleArgument(const std::shared_ptr<peg::Ast> &ast) const;

		Prototype *visitStructure(const std::shared_ptr<peg::Ast> &ast) const;
		GlobalNode *visitGlobal(const std::shared_ptr<peg::Ast> &ast) const;
		Namespace *visitNamespace(const std::shared_ptr<peg::Ast> &ast) const;
		Use *visitUse(const std::shared_ptr<peg::Ast> &ast) const;

		BlockInstructionNode *visitReturn(const std::shared_ptr<peg::Ast> &ast) const;
		BlockInstructionNode *visitBlock(const std::shared_ptr<peg::Ast> &ast) const;

		ITypeName *visitTypeName(const std::shared_ptr<peg::Ast> &ast) const;
		ITypeName *visitRegularType(const std::shared_ptr<peg::Ast> &ast) const;
		ITypeName *visitFunctionType(const std::shared_ptr<peg::Ast> &ast) const;

		PrototypeFieldEmbryo visitField(const std::shared_ptr<peg::Ast> &ast) const;
		PrototypeFieldEmbryo visitStandardField(const std::shared_ptr<peg::Ast> &ast) const;
		PrototypeFieldEmbryo visitDecoratedField(const std::shared_ptr<peg::Ast> &ast) const;

		std::vector<std::string> m_namespaces;

		const Type *m_iteratorType;

		const Node *m_alpha;
		const Node *m_beta;
};

#endif // VISITORV2_H
