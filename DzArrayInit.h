#ifndef DZARRAYINIT_H
#define DZARRAYINIT_H

#include "DzValue.h"

class DzArrayInit : public DzValue
{
	public:
		DzArrayInit(DzValue *consumer);

		int order(const EntryPoint &entryPoint) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		DzValue *m_consumer;
};

#endif // DZARRAYINIT_H