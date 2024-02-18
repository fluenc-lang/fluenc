#pragma once

#include "expression.hpp"
#include "fragment.hpp"

class BaseValue;

class StaticNode : public fragment
{
	friend class Emitter;

	public:
		StaticNode(const BaseValue *value, const fluenc::expression_t& consumer, const fragment* next);

		std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override;

	private:
		const BaseValue *m_value;
		const fluenc::expression_t m_consumer;
		const fragment* m_next;
};
