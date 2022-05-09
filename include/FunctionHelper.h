#ifndef FUNCTIONHELPER_H
#define FUNCTIONHELPER_H

#include <tuple>
#include <vector>
#include <string>
#include <numeric>

#include "EntryPoint.h"

#include "values/UserTypeValue.h"
#include "values/TupleValue.h"
#include "values/ExpandedValue.h"
#include "values/NamedValue.h"

class Stack;

class FunctionHelper
{
	public:
		static std::tuple<int8_t, const EntryPoint *, Stack> tryCreateTailCall(const EntryPoint &entryPoint
			, const Stack &values
			, const std::vector<std::string>::const_iterator &name
			, const std::vector<std::string>::const_iterator &end
			);

		template<typename Container>
		static const EntryPoint *findTailCallTarget(const EntryPoint *candidate, Container container)
		{
			return std::accumulate(container.begin(), container.end(), candidate, [&](const EntryPoint *target, auto value) -> const EntryPoint *
			{
				if (auto userTypeValue = dynamic_cast<const UserTypeValue *>(value))
				{
					auto fields = userTypeValue->fields();

					std::vector<const BaseValue *> values;

					std::transform(begin(fields), end(fields), std::back_inserter(values), [](auto field)
					{
						return field->value();
					});

					return findTailCallTarget(target, values);
				}

				if (auto tupleValue = dynamic_cast<const TupleValue *>(value))
				{
					return findTailCallTarget(target
						, tupleValue->values()
						);
				}

				if (auto expandedValue = dynamic_cast<const ExpandedValue *>(value))
				{
					auto provider = expandedValue->provider();

					if (provider->depth() < target->depth())
					{
						return provider->entry();
					}
				}

				return target;
			});
		}
};

#endif // FUNCTIONHELPER_H
