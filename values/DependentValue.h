#ifndef DEPENDENTVALUE_H
#define DEPENDENTVALUE_H

#include "BaseValue.h"

class DependentValue : public BaseValue
{
	public:
		DependentValue(const EntryPoint *provider);

		const Type *type() const override;
		const BaseValue *clone(const EntryPoint &entryPoint) const override;

		const EntryPoint *provider() const;

	private:
		const EntryPoint *m_provider;
};

#endif // DEPENDENTVALUE_H
