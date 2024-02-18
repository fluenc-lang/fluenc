#pragma once

#include "fragment.hpp"

class RenderedIteratorValue : public fragment
{
	public:
		RenderedIteratorValue(EntryPoint *entryPoint, const std::vector<DzResult> &results);

		std::vector<DzResult> build(const Emitter &emitter, DefaultVisitorContext context) const override;

	private:
		EntryPoint *m_entryPoint;

		std::vector<DzResult> m_results;
};
