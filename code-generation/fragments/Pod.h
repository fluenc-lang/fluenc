#pragma once

#include "fragment.hpp"

#include "Stack.h"

class Pod : public fragment
{
	public:
		Pod(fragment *subject, const Stack &outputValues);

		std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override;

	private:
		fragment* m_subject;

		const Stack m_outputValues;
};
