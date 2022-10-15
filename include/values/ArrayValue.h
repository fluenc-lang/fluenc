#ifndef ARRAYVALUE_H
#define ARRAYVALUE_H

#include "Node.h"
#include "IIteratable.h"

class IteratorValue;
class ReferenceValue;
class LazyValue;

class ArrayValue : public IIteratable
{
	friend class Emitter;

	public:
		ArrayValue(const std::shared_ptr<peg::Ast> &ast
			, const ReferenceValue *index
			, const Type *type
			, const std::vector<DzResult> &values
			, size_t size
			);

		std::vector<DzResult> accept(const Emitter &visitor, DefaultVisitorContext context) const override;

	private:
		static const Node *createIterator(const IIteratable *parent, const Type *type, size_t size, const std::shared_ptr<peg::Ast> &ast);

		const std::shared_ptr<peg::Ast> m_ast;

		const ReferenceValue *m_index;
		const Node *m_iterator;

		const std::vector<DzResult> m_values;
};

#endif // ARRAYVALUE_H
