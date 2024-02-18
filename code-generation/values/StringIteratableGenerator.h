#ifndef STRINGITERATABLEGENERATOR_H
#define STRINGITERATABLEGENERATOR_H

#include <llvm/IR/Value.h>

#include "values/ILazyValueGenerator.h"

class Node;

class StringIteratableGenerator : public ILazyValueGenerator
{
	public:
        StringIteratableGenerator(const reference_value *address, const reference_value *length);

		const fragment *generate(const EntryPoint &entryPoint, GenerationMode mode) const override;

                const ILazyValueGenerator *clone(const EntryPoint &entryPoint, clone_strategy strategy) const override;

		const Type *type() const override;

	private:
                const reference_value *m_address;
                const reference_value *m_length;
};


#endif // STRINGITERATABLEGENERATOR_H
