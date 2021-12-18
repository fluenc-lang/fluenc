#ifndef TAINTEDSINK_H
#define TAINTEDSINK_H

#include "DzValue.h"

class TaintedSink : public DzValue
{
	public:
		TaintedSink(DzValue *subject);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		DzValue *m_subject;
};

#endif // TAINTEDSINK_H
