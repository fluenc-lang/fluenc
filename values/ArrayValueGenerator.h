#ifndef ARRAYVALUEGENERATOR_H
#define ARRAYVALUEGENERATOR_H

#include "ILazyValueGenerator.h"
#include "DzValue.h"

class DzValue;

class ArrayValueGenerator : public ILazyValueGenerator
{
	public:
		ArrayValueGenerator(size_t id
			, const DzValue *iterator
			, const std::vector<DzResult> &values
			);

		IIteratable *generate(const EntryPoint &entryPoint) const override;

	private:
		size_t m_id;

		const DzValue *m_iterator;

		const std::vector<DzResult> m_values;
};

#endif // ARRAYVALUEGENERATOR_H
