#ifndef ITERATORVALUEGENERATORPROXY_H
#define ITERATORVALUEGENERATORPROXY_H

#include <vector>

#include "containers/sequence.h"

#include "ILazyValueGenerator.h"
#include "DzResult.h"

class IteratorValueGeneratorProxy : public ILazyValueGenerator
{
	using strategy_t = const fragment * (IteratorValueGeneratorProxy::*)(const EntryPoint &, GenerationMode) const;

	public:
		IteratorValueGeneratorProxy(const ILazyValueGenerator *subject, EntryPoint *entryPoint, const std::vector<DzResult> &results);

		const fragment *generate(const EntryPoint &entryPoint, GenerationMode mode) const override;

                const ILazyValueGenerator *clone(const EntryPoint &entryPoint, clone_strategy strategy) const override;

		const Type *type() const override;

	private:
		const fragment *createNew(const EntryPoint &entryPoint, GenerationMode mode) const;
		const fragment *useRendered(const EntryPoint &entryPoint, GenerationMode mode) const;

		mutable sequence<strategy_t> m_strategies;

		const ILazyValueGenerator *m_subject;

		EntryPoint *m_entryPoint;

		std::vector<DzResult> m_results;
};

#endif // ITERATORVALUEGENERATORPROXY_H
