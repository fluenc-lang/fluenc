#ifndef ITERATORVALUEGENERATOR_H
#define ITERATORVALUEGENERATOR_H

#include "ILazyValueGenerator.h"

class DzValue;

class IteratorValueGenerator : public ILazyValueGenerator
{
	public:
		IteratorValueGenerator(const DzValue *subject, const EntryPoint &entryPoint);

		IIteratable *generate(const EntryPoint &entryPoint) const override;

	private:
		const DzValue *m_subject;
		const EntryPoint *m_entryPoint;
};

#endif // ITERATORVALUEGENERATOR_H
