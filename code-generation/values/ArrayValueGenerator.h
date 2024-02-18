#ifndef ARRAYVALUEGENERATOR_H
#define ARRAYVALUEGENERATOR_H

#include <vector>

#include "ILazyValueGenerator.h"
#include "DzResult.h"

class Node;

class ArrayValueGenerator : public ILazyValueGenerator
{
	public:
		ArrayValueGenerator(const std::vector<DzResult> &values, const std::shared_ptr<peg::Ast> &ast, size_t size);

		const fragment *generate(const EntryPoint &entryPoint, GenerationMode mode) const override;

                const ILazyValueGenerator *clone(const EntryPoint &entryPoint, clone_strategy strategy) const override;

		const Type *type() const override;

		size_t size() const;

		const BaseValue *elementAt(size_t index) const;

	private:
		const std::vector<DzResult> m_values;
		const std::shared_ptr<peg::Ast> m_ast;

		size_t m_size;
};

#endif // ARRAYVALUEGENERATOR_H
