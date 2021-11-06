#ifndef DZEXPORTEDFUNCTIONTERMINATOR_H
#define DZEXPORTEDFUNCTIONTERMINATOR_H

#include "DzValue.h"

class DzExportedFunctionTerminator : public DzValue
{
	public:
		int compare(DzValue *other, const EntryPoint &entryPoint) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;
};

#endif // DZEXPORTEDFUNCTIONTERMINATOR_H
