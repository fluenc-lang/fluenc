#ifndef DEPENDENTVALUE_H
#define DEPENDENTVALUE_H

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/DerivedTypes.h>

#include "values/BaseValue.h"

class Type;
class EntryPoint;
class DzValue;

class DependentValue : public BaseValue
{
	public:
		DependentValue(const Type *type, const EntryPoint *provider, const DzValue *chain);

		const Type *type() const override;

		const EntryPoint *provider() const;
		const DzValue *chain() const;

	private:
		const Type *m_type;
		const EntryPoint *m_provider;
		const DzValue *m_chain;
};

#endif // DEPENDENTVALUE_H
