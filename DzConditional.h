#ifndef DZCONDITIONAL_H
#define DZCONDITIONAL_H

#include "DzValue.h"

class DzConditional : public DzValue
{
	public:
		DzConditional(DzValue *ifFalse, DzValue *ifTrue);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		DzValue *m_ifTrue;
		DzValue *m_ifFalse;
};

#endif // DZCONDITIONAL_H
