#ifndef BLOCKSTACKFRAME_H
#define BLOCKSTACKFRAME_H

#include "DzValue.h"

class BlockStackFrame : public DzValue
{
	public:
		BlockStackFrame(DzValue *consumer);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		DzValue *m_consumer;
};

#endif // BLOCKSTACKFRAME_H
