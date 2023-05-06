#ifndef STRINGITERATABLEGENERATOR_H
#define STRINGITERATABLEGENERATOR_H

#include <llvm/IR/Value.h>

#include "values/ILazyValueGenerator.h"

class Node;

class StringIteratableGenerator : public ILazyValueGenerator
{
	public:
		StringIteratableGenerator(llvm::Value *address, const ReferenceValue *length);

		const IIteratable *generate(const EntryPoint &entryPoint, GenerationMode mode) const override;

		const ILazyValueGenerator *clone(const EntryPoint &entryPoint, CloneStrategy strategy) const override;

		const Type *type() const override;

	private:
		llvm::Value *m_address;

		const ReferenceValue *m_length;
};


#endif // STRINGITERATABLEGENERATOR_H
