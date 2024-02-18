#ifndef ITERATORVALUEGENERATOR_H
#define ITERATORVALUEGENERATOR_H

#include "ILazyValueGenerator.h"

#include "expression.hpp"

class Node;

struct stack_segment_node;

class IteratorValueGenerator : public ILazyValueGenerator
{
	public:
		IteratorValueGenerator(const Type *iteratorType, stack_segment_node* subject, const EntryPoint &entryPoint);

		const fragment *generate(const EntryPoint &entryPoint, GenerationMode mode) const override;

		const ILazyValueGenerator *clone(const EntryPoint &entryPoint, clone_strategy strategy) const override;

		const Type *type() const override;

		stack_segment_node *subject() const;

	private:
		const Type *m_iteratorType;
		stack_segment_node* m_subject;
		const EntryPoint *m_entryPoint;
};

#endif // ITERATORVALUEGENERATOR_H
