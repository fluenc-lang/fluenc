#ifndef TUPLEVALUE_H
#define TUPLEVALUE_H

#include <sstream>
#include <numeric>

#include "BaseValue.h"
#include "Utility.h"
#include "Stack.h"

#include "types/TupleType.h"

class TupleValue : public BaseValue
{
	public:
		TupleValue(const std::vector<const BaseValue *> &values);

		const Type *type() const override
		{
			std::vector<const Type *> types;

			std::transform(begin(m_values), end(m_values), std::back_insert_iterator(types), [](auto value)
			{
				return value->type();
			});

			return TupleType::get(types);
		}

		Stack values() const
		{
			return m_values;
		}

	private:
		std::vector<const BaseValue *> m_values;
};

#endif // TUPLEVALUE_H
