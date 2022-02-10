#ifndef REFERENCESINK_H
#define REFERENCESINK_H

#include "DzValue.h"

class ReferenceSink : public DzValue
{
	public:
		ReferenceSink(const DzValue *consumer);

		const BaseValue *makeReference(const BaseValue *value, const EntryPoint &entryPoint) const;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const DzValue *m_consumer;
};

#endif // REFERENCESINK_H
