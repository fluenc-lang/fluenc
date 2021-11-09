#ifndef DZARRAYELEMENT_H
#define DZARRAYELEMENT_H

#include "DzValue.h"

class DzArrayElement : public DzValue
{
	public:
		DzArrayElement(size_t index, DzValue *consumer, DzValue *next);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		size_t m_index;

		DzValue *m_consumer;
		DzValue *m_next;
};
#endif // DZARRAYELEMENT_H
