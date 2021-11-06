#ifndef DZRETURN_H
#define DZRETURN_H

#include "DzValue.h"

class DzReturn : public DzValue
{
	public:
		DzReturn(DzValue *consumer, DzValue *chained);

		int compare(DzValue *other, const EntryPoint &entryPoint) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		DzValue *m_consumer;
		DzValue *m_chained;
};

#endif // DZRETURN_H
