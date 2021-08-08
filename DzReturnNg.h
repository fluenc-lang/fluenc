#ifndef DZRETURNNG_H
#define DZRETURNNG_H

#include "DzValueNg.h"

class DzReturnNg : public DzValueNg
{
	public:
		DzReturnNg(DzValueNg *value, DzValueNg *consumer);

		llvm::Value *build(const EntryPointInfo &entryPoint) const override;

	private:
		DzValueNg *m_value;
		DzValueNg *m_consumer;
};

#endif // DZRETURNNG_H
