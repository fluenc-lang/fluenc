#ifndef STRINGITERATABLEGENERATOR_H
#define STRINGITERATABLEGENERATOR_H

#include <llvm/IR/Value.h>

#include "values/ILazyValueGenerator.h"

class StringIteratableGenerator : public ILazyValueGenerator
{
	public:
		StringIteratableGenerator(llvm::Value *address, size_t id, size_t length);

		const IIteratable *generate(const EntryPoint &entryPoint) const override;

		const ILazyValueGenerator *clone(const EntryPoint &entryPoint) const override;
		const ILazyValueGenerator *forward(size_t id) const override;

		const Type *type() const override;

	private:
		llvm::Value *m_address;

		size_t m_id;
		size_t m_length;
};


#endif // STRINGITERATABLEGENERATOR_H
