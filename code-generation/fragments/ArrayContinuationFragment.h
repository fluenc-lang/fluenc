#pragma once

#include "fragment.hpp"

class reference_value;
class Type;

class ArrayContinuationNode : public fragment
{
	friend class Emitter;

	public:
        ArrayContinuationNode(const reference_value *index, const fragment *node, const Type *iteratorType);

		std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override;

	private:
                const reference_value *m_index;

		const fragment* m_node;
		const Type *m_iteratorType;
};
