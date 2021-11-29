#ifndef DZEMPTYARRAY_H
#define DZEMPTYARRAY_H

#include "DzValue.h"

class DzEmptyArray : public DzValue
{
	public:
		DzEmptyArray(DzValue *consumer);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		DzValue *m_consumer;
};

#endif // DZEMPTYARRAY_H
