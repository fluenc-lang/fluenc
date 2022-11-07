#ifndef VALUEHELPER_H
#define VALUEHELPER_H

#include "EntryPoint.h"

#include "values/TupleValue.h"

class BaseValue;
class ScalarValue;
class Emitter;

class ValueHelper
{
	public:
		static EntryPoint transferValue(const EntryPoint &entryPoint
			, const BaseValue *value
			, const BaseValue *storage
			, const Emitter &emitter
			);

		static const ScalarValue *getScalar(const EntryPoint &entryPoint, const BaseValue *value);
		static const ScalarValue *getScalar(const EntryPoint &entryPoint, Stack &values);

		template<typename T, typename TContainer>
		static std::vector<const T *> extractValues(TContainer container)
		{
			std::vector<const T *> values;

			for (auto i = container.rbegin(); i != container.rend(); i++)
			{
				if (auto tupleValue = dynamic_cast<const TupleValue *>(*i))
				{
					auto nested = extractValues<T>(tupleValue->values());

					for (auto &value : nested)
					{
						values.push_back(value);
					}
				}

				if (auto toFind = dynamic_cast<const T *>(*i))
				{
					values.push_back(toFind);
				}
			}

			return values;
		}
};

#endif // VALUEHELPER_H
