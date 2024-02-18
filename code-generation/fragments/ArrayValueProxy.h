#pragma once

#include "fragment.hpp"

class reference_value;

class ArrayValueProxy : public fragment
{
	public:
        ArrayValueProxy(const reference_value *index, const fragment *subject);

		std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override;

	private:
                const reference_value *m_index;
		const fragment *m_subject;
};
