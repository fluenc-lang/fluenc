#ifndef ARRAYVALUEGENERATOR_H
#define ARRAYVALUEGENERATOR_H

#include "ILazyValueGenerator.h"
#include "Node.h"

class Node;

class ArrayValueGenerator : public ILazyValueGenerator
{
	public:
		ArrayValueGenerator(const std::vector<DzResult> &values, size_t id, size_t size);

		const IIteratable *generate(const EntryPoint &entryPoint) const override;

		const Type *type() const override;

	private:
		const std::vector<DzResult> m_values;

		size_t m_id;
		size_t m_size;
};

#endif // ARRAYVALUEGENERATOR_H
