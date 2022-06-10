#ifndef ARRAYVALUE_H
#define ARRAYVALUE_H

#include "BaseValue.h"
#include "Node.h"
#include "IIteratable.h"

class IteratorValue;
class ReferenceValue;
class LazyValue;

class ArrayValue : public IIteratable
{
	public:
		ArrayValue(const ReferenceValue *index
			, const Type *type
			, const std::vector<DzResult> &values
			, size_t size
			);

		std::vector<DzResult> build(const EntryPoint &entryPoint) const override;

	private:
		static const Node *createIterator(const IIteratable *parent, const Type *type, size_t size);

		const ReferenceValue *m_index;
		const Node *m_iterator;

		const std::vector<DzResult> m_values;
};

#endif // ARRAYVALUE_H
