#ifndef VALUEHELPER_H
#define VALUEHELPER_H

#include "EntryPoint.h"

#include "values/TupleValue.h"

class BaseValue;
class scalar_value;
class Emitter;

class ValueHelper
{
	public:
		static EntryPoint transferValue(const EntryPoint &entryPoint
			, const BaseValue *value
			, const BaseValue *storage
			, const Emitter &emitter
			);

                static const scalar_value *getScalar(const std::shared_ptr<peg::Ast> &ast, const EntryPoint &entryPoint, const BaseValue *value);
                static const scalar_value *getScalar(const std::shared_ptr<peg::Ast> &ast, const EntryPoint &entryPoint, Stack &values);

		template<typename T, typename TContainer>
		static std::vector<const T *> extractValues(TContainer container)
		{
			std::vector<const T *> values;

			for (auto i = container.rbegin(); i != container.rend(); i++)
			{
                            if (auto tupleValue = value_cast<const tuple_value *>(*i))
				{
					auto nested = extractValues<T>(tupleValue->values());

					for (auto &value : nested)
					{
						values.push_back(value);
					}
				}

				if (auto toFind = value_cast<const T *>(*i))
				{
					values.push_back(toFind);
				}
			}

			return values;
		}
};

#endif // VALUEHELPER_H
