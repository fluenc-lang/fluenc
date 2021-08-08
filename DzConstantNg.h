#ifndef DZCONSTANTNG_H
#define DZCONSTANTNG_H

#include "DzValueNg.h"

class DzConstantNg : public DzValueNg
{
	public:
		DzConstantNg(const std::string &value);

		llvm::Value *build(const EntryPointInfo &entryPoint) const override;

	private:
		std::string m_value;
};

#endif // DZCONSTANTNG_H
