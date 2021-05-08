#ifndef DZRETURN_H
#define DZRETURN_H

#include "DzValue.h"

class DzReturn : public DzValue
{
	public:
		DzReturn(DzValue *value);

		DzValue *value() const;

		llvm::Value *build(const EntryPoint &entryPoint) const override;

	private:
		DzValue *m_value;
};

#endif // DZRETURN_H
