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
		IIteratable *resolveIteratable(const LazyValue *lazy, const EntryPoint &entryPoint) const;

		std::vector<DzResult> digestDepth(const EntryPoint &entryPoint, Stack values) const;

		mutable std::unordered_map<size_t, IIteratable *> m_generated;

		const DzValue *m_consumer;
};

#endif // LAZYEVALUATION_H
