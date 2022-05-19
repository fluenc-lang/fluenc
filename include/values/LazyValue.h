#ifndef LAZYVALUE_H
#define LAZYVALUE_H

#include "BaseValue.h"
#include "Node.h"

#include "metadata/LazyValueMetadata.h"

class ILazyValueGenerator;
class IIteratable;

class LazyValue : public BaseValueWithMetadata<LazyValueMetadata>
{
	public:
		LazyValue(const ILazyValueGenerator *generator);

		const Type *type() const override;

		const BaseValue *clone(const EntryPoint &entryPoint) const override;
		const BaseValue *forward(size_t id) const override;

		const IIteratable *generate(const EntryPoint &entryPoint) const;

		EntryPoint assignFrom(const EntryPoint &entryPoint, const LazyValue *source) const;

	private:
		const ILazyValueGenerator *m_generator;
};

#endif // LAZYVALUE_H
