#ifndef DZPARAMETER_H
#define DZPARAMETER_H

#include "DzValue.h"

class DzParameter : public DzValue
{
	public:
		DzParameter(int id);

		llvm::Value *build(const EntryPoint &entryPoint) const override;

	private:
		int m_id;
};

#endif // DZPARAMETER_H
