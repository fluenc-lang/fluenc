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
		LazyValue(const ILazyValueGenerator *generator, const EntryPoint &entryPoint);

		ValueId id() const override;

		const Type *type() const override;

		const BaseValue *clone(const EntryPoint &entryPoint, CloneStrategy strategy) const override;
		const BaseValue *forward(size_t id) const override;

		const IIteratable *generate(const EntryPoint &entryPoint) const;

		EntryPoint assignFrom(const EntryPoint &entryPoint, const BaseValue *source, const Emitter &emitter) const;
		EntryPoint assignFrom(const EntryPoint &entryPoint, const LazyValue *source, const Emitter &emitter) const;

	private:
		const ILazyValueGenerator *m_generator;
		const EntryPoint *m_entryPoint;
};

#endif // LAZYVALUE_H
