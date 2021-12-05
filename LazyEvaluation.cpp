#include "LazyEvaluation.h"

#include "values/LazyValue.h"

std::vector<DzResult> LazyEvaluation::build(const EntryPoint &entryPoint, Stack values) const
{
	if (values.size() > 0)
	{
		auto value = values.pop();

		if (auto lazy = dynamic_cast<const LazyValue *>(value))
		{
			auto block = entryPoint.block();

			return lazy->build(block, values);
		}

		values.push(value);
	}

	return {{ entryPoint, values }};
}
