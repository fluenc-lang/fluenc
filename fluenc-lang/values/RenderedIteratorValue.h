#ifndef RENDEREDITERATORVALUE_H
#define RENDEREDITERATORVALUE_H

#include "IIteratable.h"

class RenderedIteratorValue : public IIteratable
{
	public:
		RenderedIteratorValue(EntryPoint *entryPoint, const std::vector<DzResult> &results);

		std::vector<DzResult> accept(const Emitter &emitter, DefaultVisitorContext context) const override;

	private:
		EntryPoint *m_entryPoint;

		std::vector<DzResult> m_results;
};

#endif // RENDEREDITERATORVALUE_H
