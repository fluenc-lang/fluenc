#ifndef ARRAYSINK_H
#define ARRAYSINK_H

#include "DzValue.h"

class Type;

class ArraySink : public DzValue
{
	public:
		ArraySink(const Type *iteratorType
			, const DzValue *consumer
			, const DzValue *subject
			);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const Type *m_iteratorType;

		const DzValue *m_consumer;
		const DzValue *m_subject;
};

#endif // ARRAYSINK_H
