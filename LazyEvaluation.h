#ifndef LAZYEVALUATION_H
#define LAZYEVALUATION_H

#include "DzValue.h"

class IIteratable;
class LazyValue;

class LazyEvaluation : public DzValue
{
	public:
		LazyEvaluation(const DzValue *consumer);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		std::vector<DzResult> digestDepth(const EntryPoint &entryPoint, Stack values) const;

		const DzValue *m_consumer;
};

#endif // LAZYEVALUATION_H
