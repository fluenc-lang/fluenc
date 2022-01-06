#ifndef LAZYSINK_H
#define LAZYSINK_H

#include "DzValue.h"
#include "IteratorTypeHandle.h"

class LazySink : public DzValue
{
	public:
		LazySink(const IteratorTypeHandle &handle, const DzValue *consumer, const DzValue *subject);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const IteratorTypeHandle m_handle;

		const DzValue *m_consumer;
		const DzValue *m_subject;
};

#endif // LAZYSINK_H
