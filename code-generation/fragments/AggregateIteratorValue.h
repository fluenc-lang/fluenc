#pragma once

#include "fragment.hpp"
#include "expression.hpp"

class reference_value;
class Type;
class BaseValue;

class AggregateIteratorValue : public fragment
{
	public:
		AggregateIteratorValue(const reference_value *index, const fluenc::expression_t &node, const Type *arrayType, const std::vector<const BaseValue *> &iteratables);

		std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override;

	private:
		const reference_value *m_index;
		const fluenc::expression_t m_node;
		const Type *m_arrayType;

		std::vector<const BaseValue *> m_values;
};
