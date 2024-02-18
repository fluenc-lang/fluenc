#ifndef AGGREGATEITERATORVALUEGENERATOR_H
#define AGGREGATEITERATORVALUEGENERATOR_H

#include <vector>

#include "ILazyValueGenerator.h"

#include "expression.hpp"

struct ArrayBinaryNode;

class AggregateIteratorValueGenerator : public ILazyValueGenerator
{
	public:
		AggregateIteratorValueGenerator(const fluenc::expression_t &node, const std::vector<const BaseValue *> &input);

		const fragment *generate(const EntryPoint &entryPoint, GenerationMode mode) const override;

                const ILazyValueGenerator *clone(const EntryPoint &entryPoint, clone_strategy strategy) const override;

		const Type *type() const override;

	private:
		fluenc::expression_t m_node;

		std::vector<const BaseValue *> m_input;
};

#endif // AGGREGATEITERATORVALUEGENERATOR_H
