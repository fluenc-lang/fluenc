#ifndef DZMEMBERACCESS_H
#define DZMEMBERACCESS_H

#include "DzValue.h"

class DzMemberAccess : public DzValue
{
	public:
		DzMemberAccess(std::string name);

		llvm::Value *build(const EntryPoint &context) const override;

	private:
		std::string m_name;
};
#endif // DZMEMBERACCESS_H
