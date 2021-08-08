#ifndef DZFUNCTIONTERMINATOR_H
#define DZFUNCTIONTERMINATOR_H

#include "DzValue.h"

class DzFunctionTerminator : public DzValue
{
	public:
		DzFunctionTerminator(const std::string &name);

		llvm::Value *build(const EntryPoint &entryPoint, std::deque<llvm::Value *> &values) const override;

	private:
		std::string m_name;
};

#endif // DZFUNCTIONTERMINATOR_H
