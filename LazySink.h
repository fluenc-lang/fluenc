#ifndef LAZYSINK_H
#define LAZYSINK_H

#include "DzValue.h"

class LazySink : public DzValue
{
	public:
		LazySink(const DzValue *consumer, const DzValue *subject);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const DzValue *m_consumer;
		const DzValue *m_subject;
};

#endif // LAZYSINK_H
