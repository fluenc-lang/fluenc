#ifndef DZEXPANSION_H
#define DZEXPANSION_H

#include "DzValue.h"

class DzExpansion : public DzValue
{
	public:
		DzExpansion(DzValue *consumer);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		DzValue *m_consumer;
};

#endif // DZEXPANSION_H
