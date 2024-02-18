#pragma once

#include "fragment.hpp"

class ReadOnlyIteratorValue : public fragment
{
	public:
		ReadOnlyIteratorValue(const std::vector<DzResult>& results);

		std::vector<DzResult> build(const Emitter &emitter, DefaultVisitorContext context) const override;

	private:
		std::vector<DzResult> m_results;
};
