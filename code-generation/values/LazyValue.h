#ifndef LAZYVALUE_H
#define LAZYVALUE_H

#include "BaseValue.h"

#include "metadata/LazyValueMetadata.h"

class ILazyValueGenerator;
class IIteratable;
class Emitter;

class fragment;

class lazy_value : public BaseValueWithMetadata<LazyValueMetadata>
{
	public:
        lazy_value(const ILazyValueGenerator *generator, const EntryPoint &entryPoint);

                value_id id() const override;

		const Type *type() const override;

                const BaseValue *clone(const EntryPoint &entryPoint, clone_strategy strategy) const override;

		const fragment *generate(const EntryPoint &entryPoint) const;

		EntryPoint assignFrom(const EntryPoint &entryPoint, const BaseValue *source, const Emitter &emitter) const;
                EntryPoint assignFrom(const EntryPoint &entryPoint, const lazy_value *source, const Emitter &emitter) const;

	private:
		const ILazyValueGenerator *m_generator;
		const EntryPoint *m_entryPoint;

		mutable const Type *m_type;
};

#endif // LAZYVALUE_H
