#ifndef DZBINARYNG_H
#define DZBINARYNG_H

#include "DzValueNg.h"

class DzBinaryNg : public DzValueNg
{
	public:
		DzBinaryNg(const std::string &op
			, DzValueNg *left
			, DzValueNg *right
			);

		llvm::Value *build(const EntryPointInfo &entryPoint) const override;

	private:
		std::string m_op;

		DzValueNg *m_left;
		DzValueNg *m_right;
};
#endif // DZBINARYNG_H
