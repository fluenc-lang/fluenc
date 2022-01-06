#ifndef JUNCTION_H
#define JUNCTION_H

#include "DzValue.h"

class Junction : public DzValue
{
	struct SingleResult
	{
		const EntryPoint entryPoint;
		const BaseValue *value;
	};

	public:
		Junction(DzValue *subject);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const BaseValue *join(const std::vector<SingleResult> &range, const EntryPoint &entryPoint) const;
		SingleResult tryJoin(const std::vector<SingleResult> &values, const EntryPoint &entryPoint) const;

		DzValue *m_subject;
};

#endif // JUNCTION_H
