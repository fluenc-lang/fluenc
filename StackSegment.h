#ifndef STACKSEGMENT_H
#define STACKSEGMENT_H

#include "DzValue.h"

class StackSegment : public DzValue
{
	public:
		StackSegment(DzValue *subject, DzValue *consumer);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		DzValue *m_subject;
		DzValue *m_consumer;
};

#endif // STACKSEGMENT_H
