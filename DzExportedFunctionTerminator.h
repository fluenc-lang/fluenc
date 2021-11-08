#ifndef DZEXPORTEDFUNCTIONTERMINATOR_H
#define DZEXPORTEDFUNCTIONTERMINATOR_H

#include "DzValue.h"

class DzExportedFunctionTerminator : public DzValue
{
	public:
		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;
};

#endif // DZEXPORTEDFUNCTIONTERMINATOR_H
