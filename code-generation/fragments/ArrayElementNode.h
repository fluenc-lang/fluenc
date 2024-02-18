#pragma once

#include <numeric>
#include <memory>

#include "fragment.hpp"
#include "token_info.hpp"

class Type;
class IIteratable;

class ArrayElementNode : public fragment
{
	friend class Emitter;

	public:
		ArrayElementNode(const std::shared_ptr<peg::Ast> &ast
			, const Type *arrayType
			, const fragment* node
			, const fragment* next
			);

		std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override;

	private:
		const std::shared_ptr<peg::Ast> m_ast;

		const Type *m_arrayType;
		const fragment *m_node;
		const fragment* m_next;
};
