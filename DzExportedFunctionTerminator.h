#ifndef DZEXPORTEDFUNCTIONTERMINATOR_H
#define DZEXPORTEDFUNCTIONTERMINATOR_H

#include "DzValue.h"

class DzExportedFunctionTerminator : public DzValue
{
	public:
		llvm::Value *build(const EntryPoint &entryPoint, std::deque<llvm::Value *> &values) const override;
};

#endif // DZEXPORTEDFUNCTIONTERMINATOR_H
