#ifndef ARRAYVALUE_H
#define ARRAYVALUE_H

#include "BaseValue.h"
#include "Node.h"
#include "IIteratable.h"

class IteratorValue;
class ScalarValue;
class LazyValue;

class ArrayValue : public IIteratable
{
	public:
		ArrayValue(const ScalarValue *index
			, const Node *iterator
			, const std::vector<DzResult> &values
			);

		std::vector<DzResult> build(const EntryPoint &entryPoint) const override;

	private:
		const ScalarValue *m_index;
		const Node *m_iterator;

		const std::vector<DzResult> m_values;
};

#endif // ARRAYVALUE_H
