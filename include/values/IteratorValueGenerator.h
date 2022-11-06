#ifndef ITERATORVALUEGENERATOR_H
#define ITERATORVALUEGENERATOR_H

#include "ILazyValueGenerator.h"

#include "types/IteratorType.h"

class Node;

class IteratorValueGenerator : public ILazyValueGenerator
{
	public:
		IteratorValueGenerator(const Type *iteratorType, const Node *subject, const EntryPoint &entryPoint);

		const IIteratable *generate(const EntryPoint &entryPoint, GenerationMode mode) const override;

		const ILazyValueGenerator *clone(const EntryPoint &entryPoint) const override;
		const ILazyValueGenerator *forward(size_t id) const override;

		const Type *type() const override;

		const Node *subject() const;

	private:
		const Type *m_iteratorType;
		const Node *m_subject;
		const EntryPoint *m_entryPoint;
};

#endif // ITERATORVALUEGENERATOR_H
