#ifndef ARRAYVALUEGENERATOR_H
#define ARRAYVALUEGENERATOR_H

#include "ILazyValueGenerator.h"
#include "Node.h"

class Node;

class ArrayValueGenerator : public ILazyValueGenerator
{
	public:
		ArrayValueGenerator(const std::vector<DzResult<BaseValue>> &values, size_t id, size_t size);

		const IIteratable *generate(const EntryPoint &entryPoint) const override;

		const ILazyValueGenerator *clone(const EntryPoint &entryPoint) const override;
		const ILazyValueGenerator *forward(size_t id) const override;

		const Type *type() const override;

		size_t size() const;

		const BaseValue *elementAt(size_t index) const;

	private:
		const std::vector<DzResult<BaseValue>> m_values;

		size_t m_id;
		size_t m_size;
};

#endif // ARRAYVALUEGENERATOR_H
