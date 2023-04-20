#ifndef AGGREGATEITERATORVALUEGENERATOR_H
#define AGGREGATEITERATORVALUEGENERATOR_H

#include <vector>

#include "ILazyValueGenerator.h"

struct ArrayBinaryNode;

class AggregateIteratorValueGenerator : public ILazyValueGenerator
{
	public:
		AggregateIteratorValueGenerator(const ArrayBinaryNode *node, const std::vector<const BaseValue *> &input);

		const IIteratable *generate(const EntryPoint &entryPoint, GenerationMode mode) const override;

		const ILazyValueGenerator *clone(const EntryPoint &entryPoint, CloneStrategy strategy) const override;
		const ILazyValueGenerator *forward(size_t id) const override;

		const Type *type() const override;

	private:
		const ArrayBinaryNode *m_node;

		std::vector<const BaseValue *> m_input;
};

#endif // AGGREGATEITERATORVALUEGENERATOR_H
