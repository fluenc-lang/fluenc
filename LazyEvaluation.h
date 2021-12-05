#ifndef LAZYEVALUATION_H
#define LAZYEVALUATION_H

#include "DzValue.h"

class LazyEvaluation : public DzValue
{
	public:
		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;
};

#endif // LAZYEVALUATION_H
