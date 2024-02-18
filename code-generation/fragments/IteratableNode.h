#pragma once

#include "fragment.hpp"

class IteratableNode : public fragment
{
	friend class Emitter;

	public:
		IteratableNode(const fragment *iteratable);

		std::vector<DzResult> build(const Emitter& visitor, DefaultVisitorContext context) const override;

	private:
		const fragment* m_subject;
};
