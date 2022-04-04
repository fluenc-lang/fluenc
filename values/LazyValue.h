#ifndef LAZYVALUE_H
#define LAZYVALUE_H

#include "BaseValue.h"
#include "DzValue.h"

class ILazyValueGenerator;
class IIteratable;

class LazyValue : public BaseValue
{
	public:
		LazyValue(ILazyValueGenerator *generator);

		const Type *type() const override;
		const BaseValue *clone(const EntryPoint &entryPoint) const override;

		const IIteratable *generate(const EntryPoint &entryPoint) const;

		EntryPoint assignFrom(const EntryPoint &entryPoint, const LazyValue *source) const;

	private:
		ILazyValueGenerator *m_generator;
};

#endif // LAZYVALUE_H
