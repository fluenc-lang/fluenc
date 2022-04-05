#ifndef EXPANDABLEVALUE_H
#define EXPANDABLEVALUE_H

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/DerivedTypes.h>

#include "values/BaseValue.h"

class Type;
class EntryPoint;
class Node;
class IteratorType;

class ExpandableValue : public BaseValue
{
	public:
		ExpandableValue(const Type *iteratorType, const EntryPoint &provider, const Node *chain);

		const Type *type() const override;
		const BaseValue *clone(const EntryPoint &entryPoint) const override;

		const EntryPoint *provider() const;
		const Node *chain() const;

	private:
		const Type *m_iteratorType;
		const EntryPoint *m_provider;
		const Node *m_chain;
};

#endif // EXPANDABLEVALUE_H
