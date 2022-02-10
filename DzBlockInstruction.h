#ifndef DZBLOCKINSTRUCTION_H
#define DZBLOCKINSTRUCTION_H

#include "DzValue.h"

class DzBlockInstruction : public DzValue
{
	public:
		DzBlockInstruction(const DzValue *subject, bool containsIterator);

		bool containsIterator() const;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const DzValue *m_subject;

		bool m_containsIterator;
};

#endif // DZBLOCKINSTRUCTION_H
