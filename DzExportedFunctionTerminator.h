#ifndef DZEXPORTEDFUNCTIONTERMINATOR_H
#define DZEXPORTEDFUNCTIONTERMINATOR_H

#include "DzValue.h"

class DzExportedFunctionTerminator : public DzValue
{
	public:
		Stack build(const EntryPoint &entryPoint, Stack values) const override;
};

#endif // DZEXPORTEDFUNCTIONTERMINATOR_H
