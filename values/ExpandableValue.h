#ifndef EXPANDABLEVALUE_H
#define EXPANDABLEVALUE_H

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/DerivedTypes.h>

#include "values/BaseValue.h"

class Type;
class EntryPoint;
class DzValue;

class ExpandableValue : public BaseValue
{
	public:
		ExpandableValue(const EntryPoint &provider, const DzValue *chain);

		const Type *type() const override;

		const EntryPoint *provider() const;
		const DzValue *chain() const;

	private:
		const EntryPoint *m_provider;
		const DzValue *m_chain;
};

#endif // EXPANDABLEVALUE_H
