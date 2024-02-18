#pragma once

#include "expression.hpp"

class Type;

class DefaultNode
{
	friend class Emitter;

	public:
		DefaultNode(const Type *type, const fluenc::expression_t& consumer);

	private:
		const Type *m_type;
		fluenc::expression_t m_consumer;
};
