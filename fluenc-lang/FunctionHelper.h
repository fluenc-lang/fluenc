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
				if (auto userTypeValue = value_cast<const UserTypeValue *>(value))
				{
					auto fields = userTypeValue->fields();

					std::vector<const BaseValue *> values;

					std::transform(begin(fields), end(fields), std::back_inserter(values), [](auto field)
					{
						return field->value();
					});

					return findTailCallTarget(target, values);
				}

				if (auto tupleValue = value_cast<const TupleValue *>(value))
				{
					return findTailCallTarget(target
						, tupleValue->values()
						);
				}

				if (auto expandedValue = value_cast<const ExpandedValue *>(value))
				{
					auto provider = findTailCallTarget(expandedValue->provider()
						, expandedValue->next()
						);

					if (provider->depth() < target->depth())
					{
						return provider;
					}
				}

				return target;
			});
		}
};

#endif // FUNCTIONHELPER_H
