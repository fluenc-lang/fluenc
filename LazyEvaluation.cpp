#include "LazyEvaluation.h"

#include "values/LazyValue.h"
#include "values/ArrayValue.h"

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

		if (auto array = dynamic_cast<const ArrayValue *>(value))
		{
			auto block = entryPoint.block();

			return array->build(block, values);
		}

		values.push(value);
	}

	return {{ entryPoint, values }};
}
