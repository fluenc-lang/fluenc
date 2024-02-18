#pragma once

#include <memory>

#include "fragment.hpp"
#include "token_info.hpp"

class IteratorValue;
class reference_value;
class lazy_value;
class Type;

class ArrayValue : public fragment
{
	friend class Emitter;

	public:
		ArrayValue(const std::shared_ptr<peg::Ast> &ast
                           , const reference_value *index
			, const Type *type
			, const std::vector<DzResult> &values
			, size_t size
			);

		std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override;

	private:
		static fragment *createIterator(const fragment *parent, const Type *type, size_t size, const std::shared_ptr<peg::Ast> &ast);

		const std::shared_ptr<peg::Ast> m_ast;

                const reference_value *m_index;

		fragment* m_iterator;

		const std::vector<DzResult> m_values;
};
