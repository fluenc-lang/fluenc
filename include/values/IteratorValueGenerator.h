#ifndef ITERATORVALUEGENERATOR_H
#define ITERATORVALUEGENERATOR_H

#include "ILazyValueGenerator.h"

class Node;

class IteratorValueGenerator : public ILazyValueGenerator
{
	public:
		IteratorValueGenerator(const Node *subject, const EntryPoint &entryPoint);

		const IIteratable *generate(const EntryPoint &entryPoint) const override;

		const Type *type() const override;

	private:
		const Node *m_subject;
		const EntryPoint *m_entryPoint;
};

#endif // ITERATORVALUEGENERATOR_H
