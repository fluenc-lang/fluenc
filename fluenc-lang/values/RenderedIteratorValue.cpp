#include "Utility.h"

#include "values/RenderedIteratorValue.h"

RenderedIteratorValue::RenderedIteratorValue(EntryPoint *entryPoint, const std::vector<DzResult> &results)
	: m_entryPoint(entryPoint)
	, m_results(results)
{
}

std::vector<DzResult> RenderedIteratorValue::accept(const Emitter &emitter, DefaultVisitorContext context) const
{
	UNUSED(emitter);

	m_entryPoint->setParent(context.entryPoint);

	linkBlocks(context.entryPoint.block(), m_entryPoint->block());

	std::vector<DzResult> results;

	for (auto &[resultEntryPoint, resultValues] : m_results)
	{
		auto forwardedEntryPoint = resultEntryPoint
			.withLocals(context.entryPoint.locals());

		results.push_back({ forwardedEntryPoint, resultValues });
	}

	return results;
}
