#ifndef DZRETURN_H
#define DZRETURN_H

#include "DzValue.h"

class DzReturn : public DzValue
{
	public:
		DzReturn(DzValue *consumer);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		DzValue *m_consumer;
};

#endif // DZRETURN_H
