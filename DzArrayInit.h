#ifndef DZARRAYINIT_H
#define DZARRAYINIT_H

#include "DzValue.h"

class DzArrayInit : public DzValue
{
	public:
		DzArrayInit(const DzValue *consumer, const DzValue *subject);

		int order(const EntryPoint &entryPoint) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const DzValue *m_consumer;
		const DzValue *m_subject;
};

#endif // DZARRAYINIT_H
