#ifndef INDEXSINK_H
#define INDEXSINK_H

#include "DzValue.h"

#include "values/IndexedValue.h"

class IndexSink : public DzValue
{
	public:
		IndexSink(size_t index, const DzValue *consumer);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		size_t m_index;

		const DzValue *m_consumer;
};

#endif // INDEXSINK_H
