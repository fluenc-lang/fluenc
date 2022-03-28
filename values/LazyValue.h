#ifndef LAZYVALUE_H
#define LAZYVALUE_H

#include "BaseValue.h"
#include "DzValue.h"

class ILazyValueGenerator;
class IIteratable;

class LazyValue : public BaseValue
{
	public:
		static const size_t NoCache = -1;

		LazyValue(size_t id, ILazyValueGenerator *generator);

		size_t id() const;

		const Type *type() const override;
		const BaseValue *clone(const EntryPoint &entryPoint) const override;

		IIteratable *generate(const EntryPoint &entryPoint) const;

		EntryPoint assignFrom(const EntryPoint &entryPoint, const LazyValue *source) const;

	private:
		size_t m_id;

		ILazyValueGenerator *m_generator;
};

#endif // LAZYVALUE_H
