#ifndef DZARRAYELEMENT_H
#define DZARRAYELEMENT_H

#include "DzValue.h"

class Type;

class DzArrayElement : public DzValue
{
	public:
		DzArrayElement(const Type *iteratorType, size_t index, DzValue *next);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const Type *m_iteratorType;

		size_t m_index;

		DzValue *m_next;
};
#endif // DZARRAYELEMENT_H
