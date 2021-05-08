#ifndef DZCONSTANT_H
#define DZCONSTANT_H

#include "DzValue.h"

class DzConstant : public DzValue
{
	public:
		DzConstant(std::string value);

		llvm::Value *build(const EntryPoint &context) const override;

	private:
		std::string m_value;
};
#endif // DZCONSTANT_H
