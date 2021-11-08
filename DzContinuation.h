#ifndef DZCONTINUATION_H
#define DZCONTINUATION_H

#include "DzValue.h"

class DzContinuation : public DzValue
{
	public:
		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;
};

#endif // DZCONTINUATION_H
